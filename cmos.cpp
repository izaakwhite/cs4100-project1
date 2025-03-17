#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <cctype>

namespace fs = std::filesystem;

// --- Parameters (tweak these if you need) ---
const int K = 4; // k-mer length
const int W = 5; // window size for winnowing

// --- Helper functions ---

// Generate overlapping k-mers from a token string.
std::vector<std::string> getKmers(const std::string &tokens, int k) {
    std::vector<std::string> kmers;
    if (tokens.size() < static_cast<size_t>(k))
        return kmers;
    for (size_t i = 0; i <= tokens.size() - k; i++) {
        kmers.push_back(tokens.substr(i, k));
    }
    return kmers;
}

// Simple hash function using std::hash.
size_t hashKmer(const std::string &kmer) {
    static std::hash<std::string> hasher;
    return hasher(kmer);
}

// Compute fingerprints using a sliding window (winnowing).
// For each window of W hashed k-mers, choose the minimum hash value.
std::vector<size_t> computeFingerprints(const std::vector<size_t>& hashes, int w) {
    std::vector<size_t> fingerprints;
    if (hashes.size() < static_cast<size_t>(w))
        return fingerprints;
    for (size_t i = 0; i <= hashes.size() - w; i++) {
        size_t minVal = hashes[i];
        for (size_t j = i; j < i + w; j++) {
            if (hashes[j] < minVal) {
                minVal = hashes[j];
            }
        }
        fingerprints.push_back(minVal);
    }
    return fingerprints;
}

// Compute Jaccard similarity between two fingerprint sets.
double computeSimilarity(const std::vector<size_t>& fp1, const std::vector<size_t>& fp2) {
    std::unordered_set<size_t> set1(fp1.begin(), fp1.end());
    std::unordered_set<size_t> set2(fp2.begin(), fp2.end());
    
    size_t intersectionCount = 0;
    for (auto val : set1) {
        if (set2.find(val) != set2.end()) {
            intersectionCount++;
        }
    }
    size_t unionCount = set1.size() + set2.size() - intersectionCount;
    if (unionCount == 0) return 0.0;
    return static_cast<double>(intersectionCount) / unionCount;
}

// Structure to store submission fingerprints.
struct Submission {
    std::string filename;
    std::vector<size_t> fingerprints;
};

int main() {
    std::vector<Submission> submissions;
    std::string directory = "Examples";

    // Process each .c file in the Examples directory.
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".c") {
            std::ifstream infile(entry.path());
            if (!infile) {
                std::cerr << "Failed to open file: " << entry.path() << "\n";
                continue;
            }
            std::stringstream buffer;
            buffer << infile.rdbuf();
            std::string tokenString = buffer.str();

            // Remove all whitespace so the token string is contiguous.
            tokenString.erase(std::remove_if(tokenString.begin(), tokenString.end(),
                                             [](unsigned char c){ return std::isspace(c); }),
                              tokenString.end());

            // Break the token string into overlapping k-mers.
            std::vector<std::string> kmers = getKmers(tokenString, K);

            // Hash each k-mer.
            std::vector<size_t> hashedKmers;
            for (const auto &kmer : kmers) {
                hashedKmers.push_back(hashKmer(kmer));
            }

            // Compute fingerprints via winnowing.
            std::vector<size_t> fingerprints = computeFingerprints(hashedKmers, W);
            submissions.push_back({entry.path().filename().string(), fingerprints});
        }
    }

    // Compute pairwise similarities.
    struct Similarity {
        std::string fileA;
        std::string fileB;
        double score;
    };
    std::vector<Similarity> similarities;
    for (size_t i = 0; i < submissions.size(); i++) {
        for (size_t j = i + 1; j < submissions.size(); j++) {
            double sim = computeSimilarity(submissions[i].fingerprints, submissions[j].fingerprints);
            similarities.push_back({submissions[i].filename, submissions[j].filename, sim});
        }
    }

    // Sort similarities in descending order.
    std::sort(similarities.begin(), similarities.end(), [](const Similarity &a, const Similarity &b) {
        return a.score > b.score;
    });

    // Write the report file.
    std::ofstream report("report.txt");
    if (!report) {
        std::cerr << "Failed to open report file for writing.\n";
        return 1;
    }
    report << "Similarity Report\n";
    report << "-----------------\n";
    for (const auto &sim : similarities) {
        report << sim.fileA << " and " << sim.fileB << ": " << sim.score << "\n";
    }
    std::cout << "Report generated: report.txt\n";
    
    return 0;
}
