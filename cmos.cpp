#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <functional>
#include <algorithm>

namespace fs = std::filesystem;

// Constants for k-mer length and window size for winnowing.
const int K = 4;  // k-mer length (number of characters per k-mer)
const int W = 5;  // window size for fingerprint selection

// Reads the entire file content, and removes any whitespace.
std::string readTokenFile(const fs::path &filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file: " << filepath << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Remove all whitespace characters (spaces, tabs, newlines)
    std::string tokenString;
    for (char ch : content) {
        if (!isspace(static_cast<unsigned char>(ch))) {
            tokenString.push_back(ch);
        }
    }
    return tokenString;
}

// Given a token string, break it into overlapping k-mers, hash each,
// and then use the Winnowing Algorithm to select fingerprints.
std::unordered_set<size_t> getFingerprints(const std::string &tokenStr) {
    std::vector<size_t> hashes;
    size_t n = tokenStr.size();
    if (n < K)
        return {};

    // Use C++'s built-in hash for strings.
    std::hash<std::string> hasher;
    // Generate hashes for every k-mer
    for (size_t i = 0; i <= n - K; i++) {
        std::string kmer = tokenStr.substr(i, K);
        size_t h = hasher(kmer);
        hashes.push_back(h);
    }
    
    // Winnowing: slide a window of size W over the hash list and select the minimum in each window.
    std::unordered_set<size_t> fingerprints;
    if (hashes.size() < W) {
        // If fewer k-mers than the window size, take the min of all.
        size_t min_val = *std::min_element(hashes.begin(), hashes.end());
        fingerprints.insert(min_val);
    } else {
        for (size_t i = 0; i <= hashes.size() - W; i++) {
            auto window_start = hashes.begin() + i;
            auto window_end = window_start + W;
            size_t min_val = *std::min_element(window_start, window_end);
            fingerprints.insert(min_val);
        }
    }
    return fingerprints;
}

// Structure to hold similarity scores between two submissions.
struct SimilarityScore {
    std::string file1;
    std::string file2;
    double score;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: cmos <directory_path>\n";
        return 1;
    }
    
    fs::path directoryPath = argv[1];
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "Invalid directory: " << directoryPath << "\n";
        return 1;
    }
    
    // Read each tokenized submission in the directory.
    std::vector<std::pair<std::string, std::unordered_set<size_t>>> submissions;
    for (const auto & entry : fs::directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry.path())) {
            std::string fileContent = readTokenFile(entry.path());
            if (fileContent.empty()) continue;
            
            // Remove any whitespace (if not already removed in readTokenFile) and compute fingerprints.
            std::string tokenString;
            for (char ch : fileContent) {
                if (!isspace(static_cast<unsigned char>(ch)))
                    tokenString.push_back(ch);
            }
            std::unordered_set<size_t> fingerprints = getFingerprints(tokenString);
            submissions.push_back({ entry.path().filename().string(), fingerprints });
        }
    }
    
    if (submissions.empty()) {
        std::cerr << "No valid tokenized submission files found in the directory.\n";
        return 1;
    }
    
    // Compute similarity scores for every pair of submissions using Jaccard similarity.
    std::vector<SimilarityScore> scores;
    for (size_t i = 0; i < submissions.size(); i++) {
        for (size_t j = i + 1; j < submissions.size(); j++) {
            const auto &sub1 = submissions[i];
            const auto &sub2 = submissions[j];
            size_t intersection = 0;
            for (const auto &fp : sub1.second) {
                if (sub2.second.find(fp) != sub2.second.end()) {
                    intersection++;
                }
            }
            // Union size = sum of sizes minus intersection
            size_t unionSize = sub1.second.size() + sub2.second.size() - intersection;
            double similarity = (unionSize > 0) ? static_cast<double>(intersection) / unionSize : 0.0;
            scores.push_back({ sub1.first, sub2.first, similarity });
        }
    }
    
    // Sort pairs by similarity (highest first)
    std::sort(scores.begin(), scores.end(), [](const SimilarityScore &a, const SimilarityScore &b) {
        return a.score > b.score;
    });
    
    // Output the similarity scores for each pair.
    std::cout << "Similarity scores between submissions:\n";
    for (const auto &s : scores) {
        std::cout << s.file1 << " and " << s.file2 
                  << " --> Similarity: " << s.score << "\n";
    }
    
    // Note: Manually inspect the top-scoring pairs for potential plagiarism.
    return 0;
}
