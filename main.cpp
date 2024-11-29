#include "include/vamana.h"
#include <getopt.h>
#include <chrono>

//function to check the kind of the base file
bool ends_with(const string& str, const string& suffix) {
    if (str.size() >= suffix.size()) {
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    } else {
        return false;
    }
}

float computeRecall(const vector<int>& groundTruth, const vector<Node*>& retrievedNeighbors) {
    int truePositiveCount = 0;
    unordered_set<int> retrievedIds;

    // Store retrieved neighbor IDs in a set for fast lookup
    for (Node* node : retrievedNeighbors) {
        retrievedIds.insert(node->id);
    }

    // Count the number of true positives (common neighbors)
    for (int gtId : groundTruth) {
        if (retrievedIds.find(gtId) != retrievedIds.end()) {
            truePositiveCount++;
        }
    }

    // Recall is the fraction of true positive neighbors
    return static_cast<float>(truePositiveCount) / retrievedNeighbors.size();
}

int main(int argc, char* argv[]) {
    if (argc < 11) {
        cerr << "Usage: " << argv[0] << " -i <base.vecs> -k <k> -l <L> -r <R> -a <a>\n";
        
        return 1;
    }

    string base_file;
    string query_file;
    string groundtruth_file;
    int k = 0, L = 0, R = 0;
    float a = 0.0;

    int opt;
    while ((opt = getopt(argc, argv, "i:q:g:k:l:r:a:")) != -1) {
        switch (opt) {
            case 'i':
                base_file = optarg;
                break;
            case 'q':
                query_file = optarg;
                break;
            case 'g':
                groundtruth_file = optarg;
                break;
            case 'k':
                k = stoi(optarg);
                break;
            case 'l':
                L = stoi(optarg);
                break;
            case 'r':
                R = stoi(optarg);
                break;
            case 'a':
                a = stod(optarg);
                break;
            default:
                cerr << "Invalid arguments.\n";
                return 1;
        }
    }

    vector<vector<float>> fvecsData;
    
    if (ends_with(base_file, ".ivecs")) {
        vector<vector<int>> ivecsData = loadIvecs(base_file);
    } else if (ends_with(base_file, ".fvecs")) {
        fvecsData = loadFvecs(base_file);
    } else if (ends_with(base_file, ".bvecs")) {
        // nodes = load_bvecs(base_file);
    } else {
        cout << "Not a valid file" << endl;
        return 1;
    }

    vector<Node*> nodes = createNodesFromVectors(fvecsData);

    int n=nodes.size();
    //well connected
    if (R <= log2(n)){
        cerr << "R must be greater than log2(n), so that the graph is well connected"<<endl;
        return 1;
    }

    cout << endl << endl;
    cout << "Base file: " << base_file << endl;
    cout << "k: " << k << endl;
    cout << "L: " << L << endl;
    cout << "R: " << R << endl;
    cout << "a: " << a << endl;
    cout << endl;
    cout << "Now the implementation of the vamana algorithm is starting!" << endl;

    // Start time measurement
    auto start = chrono::high_resolution_clock::now();

    // Run the Vamana Indexing Algorithm
    DirectedGraph graph = FilteredVamana(nodes, k, L, R, a);

    // End time measurement
    auto end = chrono::high_resolution_clock::now();

    // Calculate elapsed time
    chrono::duration<float> duration = end - start;

    cout << "The vamana graph has been successfully implemented" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    vector<vector<float>> queries_vectors = loadFvecs(query_file);
    vector<Node*> queries = createNodesFromVectors(queries_vectors);

    vector<vector<int>> groundtruth = loadIvecs(groundtruth_file);

    float totalRecall = 0.0;
    int queryCount = 0;

    for (size_t i = 0; i < queries.size(); ++i) {
        Node* queryNode = queries[i];
        vector<int>& groundTruthForQuery = groundtruth[i];  // Get the ground truth for this query

        // Perform Greedy Search for the query
        int randomIndex = rand() % nodes.size();
        vector<Node*> nearestNeighbors = GreedySearch(nodes.at(randomIndex), queryNode, k, L);

        // Print nearest neighbors from GreedySearch
        cout << "Nearest neighbors from GreedySearch for query " << queryNode->id << ": ";
        for (Node* neighbor : nearestNeighbors) {
            cout << neighbor->id << " ";
        }
        cout << endl;

        // Print ground truth neighbors for the query
        cout << "Ground truth neighbors for query " << queryNode->id << ": ";
        for (int gtId : groundTruthForQuery) {
            cout << gtId << " ";
        }
        cout << endl;

        // Compute Recall for this query
        float recall = computeRecall(groundTruthForQuery, nearestNeighbors);
        totalRecall += recall;
        queryCount++;

        // Print the recall for this query
        cout << "Recall for query " << queryNode->id << ": " << recall << endl;
        cout << "--------------------------------------------------" << endl;
    }

    float averageRecall = totalRecall / queryCount;
    cout << "Average Recall: " << averageRecall << endl;

    // Cleanup: free memory
    for (Node* node : nodes) 
        delete node;

    for (Node* node : queries)
        delete node;

    return 0;
}