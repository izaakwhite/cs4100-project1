#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <cctype>

using namespace std;

// --- Parameters (tweak these if needed) ---
const int K = 4; // k-mer length
const int W = 5; // window size for winnowing

/*                */
//Helper functions//
/*                */
vector<string> getKmers(const string &tokens, const int k) {
    
    //init kmers vector
    vector<string> kmers;

    //if the input string tokens is less than k just return the initalized kmers vector
    if (tokens.size() < k) {
        return kmers;
    }

    // add kmers to the kmers vector
    for (int i = 0; i <= tokens.size() - k; i++) {
        kmers.push_back(tokens.substr(i, k));
    }
    return kmers;
}

// Simple hash function for kmer string
size_t hashKmer(const string &kmer) {
    static hash<string> hasher;
    return hasher(kmer);
}


vector<size_t> computeFingerprints(const vector<size_t>& hashes, const int w) {
    vector<size_t> fingerprints;
    if (hashes.size() < w) {
        return fingerprints;
    }

    for (int i = 0; i <= hashes.size() - w; i++) {
        int minVal = hashes[i];
        for (int j = i; j < i + w; j++) {
            if (hashes[j] < minVal) {
                minVal = hashes[j];
            }
        }
        fingerprints.push_back(minVal);
    }
    return fingerprints;
}

// Compute Jaccard similarity between two fingerprint sets.
double computeSimilarity(const vector<size_t>& fp1, const vector<size_t>& fp2) {
    unordered_set<size_t> set1(fp1.begin(), fp1.end());
    unordered_set<size_t> set2(fp2.begin(), fp2.end());
    
    size_t intersectionCount = 0;
    for (auto val : set1) {
        if (set2.find(val) != set2.end()) {
            intersectionCount++;
        }
    }
    size_t unionCount = set1.size() + set2.size() - intersectionCount;
    if (unionCount == 0)
        return 0.0;
    return static_cast<double>(intersectionCount) / unionCount;
}

// Structure to store submission fingerprints.
struct Submission {
    string filename;
    vector<size_t> fingerprints;
};

int main() {
    vector<Submission> submissions;
    string line;
    
    // Read tokens from standard input.
    // Each line: "<filename> <tokenString>"
    while (getline(cin, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        string filename, tokenString;
        if (!(iss >> filename >> tokenString))
            continue;
        
        // The token string is a contiguous string of tokens (e.g., "001002001002003033...").
        // Break the token string into overlapping k-mers.
        vector<string> kmers = getKmers(tokenString, K);
        
        // Hash each k-mer.
        vector<size_t> hashedKmers;
        for (const auto &kmer : kmers) {
            hashedKmers.push_back(hashKmer(kmer));
        }
        
        // Compute fingerprints via winnowing.
        vector<size_t> fingerprints = computeFingerprints(hashedKmers, W);
        submissions.push_back({ filename, fingerprints });
    }
    
    // Compute pairwise similarities.
    struct Similarity {
        string fileA;
        string fileB;
        double score;
    };
    vector<Similarity> similarities;
    for (size_t i = 0; i < submissions.size(); i++) {
        for (size_t j = i + 1; j < submissions.size(); j++) {
            double sim = computeSimilarity(submissions[i].fingerprints, submissions[j].fingerprints);
            similarities.push_back({ submissions[i].filename, submissions[j].filename, sim });
        }
    }
    
    // Sort similarities in descending order.
    sort(similarities.begin(), similarities.end(), [](const Similarity &a, const Similarity &b) {
        return a.score > b.score;
    });
    
    // Write the report file.
    ofstream report("PlagarismReport.txt");
    if (!report) {
        cerr << "Failed to open report file for writing.\n";
        return 1;
    }
    report << "Similarity Report\n";
    report << "-----------------\n";
    for (const auto &sim : similarities) {
        report << sim.fileA << " and " << sim.fileB << ": " << sim.score << "\n";
    }
    cout << "Report generated: PlagarismReport.txt\n";
    
    return 0;
}
