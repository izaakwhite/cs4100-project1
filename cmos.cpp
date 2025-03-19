/*
 * Izaak White & Joshua Moner
 */
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


const int K = 4; // k-mer length
const int W = 5; // window size for winnowing

vector<string> getKmers(const string &tokens, const int k) {
    
    //init kmers vector
    vector<string> kmers;

    //if the input string tokens is less than k just return the initalized kmers vector
    //as you cannot create a substring of a string if the substring must be a length larger than the string itself
    if (tokens.size() < k) {
        return kmers;
    }

    // create the kmers vector
    /*
        e.g: string = "abcdef"; k = 3
        kmers = [ 'abc', 'bcd', 'cde', 'def']
    */
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

// applies the winnowing algorithm to create a smaller hash array to make it
// less computationally expensive to compare hashes and less sensitive to small change.
// 
vector<size_t> computeFingerprints(const vector<size_t>& hashes, const int w) {
    //init fingerprints vector
    vector<size_t> fingerprints;

    // if the hashes array holds less elements than the window size then return the empty vector
    if (hashes.size() < w) {
        return fingerprints;
    }

    /*
        The winnowing algorithm. The general purpose of this algorithm is to generalize our strings as
     direct hash comparisons can make the system extremely sensitive to small changes. For example 
     adding a simple space to a document can change many of the k-mers and their corresponding hash 
     values.
        The winnowing algorithm fixes this sensitivity problem by looking at a window of hashes 
     (the window size is of size w) and selecting the minimum hash value and adding it to the
     fingerprints vector. 

    */ 
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

// Compute Jaccard similarity between two fingerprint sets. This computes |set1 ∩ set2| / | set1 ∪ set2|
double computeSimilarity(const vector<size_t>& fp1, const vector<size_t>& fp2) {

    //remove all duplicate fingerprints from fp1 and fp2
    unordered_set<size_t> set1(fp1.begin(), fp1.end());
    unordered_set<size_t> set2(fp2.begin(), fp2.end());
    
    size_t intersectionCount = 0;

    // calculate the intersection count |set1 ∩ set2|. 
    for (auto val : set1) {
        if (set2.find(val) != set2.end()) {
            intersectionCount++;
        }
    }
    // calculate the union | set1 ∪ set2| = |set1| + |set2| - |set1 ∩ set2|
    size_t unionCount = set1.size() + set2.size() - intersectionCount;

    // prevent divide by zero error
    if (unionCount == 0) {
        return 0.0;
    }

    // retrun the value of |set1 ∩ set2| / | set1 ∪ set2|
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

        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        string filename, tokenString;

        if (!(iss >> filename >> tokenString)) {
            continue;
        }

        // create a vector of kmers from the string tokenString
        vector<string> kmers = getKmers(tokenString, K);
        
        // init hashedKmers vector
        vector<size_t> hashedKmers;

        // Hash kmers 
        for (const auto &kmer : kmers) {
            hashedKmers.push_back(hashKmer(kmer));
        }
        
        // apply winnowing algorithm to compute fingerprints of hashed kmers generated from value tokenString.
        vector<size_t> fingerprints = computeFingerprints(hashedKmers, W);
        submissions.push_back({ filename, fingerprints });
    }
    
    // create similarity struct to store fileA and fileB and their comparison score.
    struct Similarity {
        string fileA;
        string fileB;
        double score;
    };
    
    vector<Similarity> similarities;
    for (int i = 0; i < submissions.size(); ++i) {
        for (int j = i + 1; j < submissions.size(); ++j) {
            //Compute Jaccard similarity between current submissions fingerprints
            double score = computeSimilarity(submissions[i].fingerprints, submissions[j].fingerprints);
            similarities.push_back({ submissions[i].filename, submissions[j].filename, score});
        }
    }
    
    //Sort similarities in descending order.
    sort(similarities.begin(), similarities.end(), [](const Similarity &a, const Similarity &b) {
        return a.score > b.score;
    });
    
    // Write to the PlagiarismReport.txt file
    ofstream report("PlagiarismReport.txt");
    if (!report) {
        cerr << "Failed to open file.\n";
        return 1;
    }
    report << "Similarity Report\n-----------------\n";
    for (const auto &sim : similarities) {
        report << sim.fileA << " and " << sim.fileB << ": " << sim.score << "\n";
    }
    
    return 0;
}
