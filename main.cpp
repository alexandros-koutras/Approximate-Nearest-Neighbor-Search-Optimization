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

int main(int argc, char* argv[]) {
    if (argc < 11) {
        cerr << "Usage: " << argv[0] << " -i <base.vecs> -q <query.vecs>-g <groundtruth.vecs> -k <k> -l <L> -r <R> -a <a>\n";
        
        return 1;
    }

    string base_file, query_file, groundtruth_file;
    int k = 0, L = 0, R = 0;
    double a = 0.0;

    int opt;
    while ((opt = getopt(argc, argv, "i:q:g:k:l:r:a:")) != -1) {
        switch (opt) {
            case 'i':
                base_file = optarg;
                break;
            case 'q':
                query_file = optarg;
            case 'g':
                groundtruth_file = optarg;
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
    

    vector<Node*> nodes;
    if (ends_with(base_file, ".ivecs")) {
        nodes = load_ivecs(base_file);
    } else if (ends_with(base_file, ".fvecs")) {
        nodes = load_fvecs(base_file);
    } else if (ends_with(base_file, ".bvecs")) {
        nodes = load_bvecs(base_file);
    } else {
        cout << "Not a valid file" << endl;
        return 1;
    }

    int n=nodes.size();
    //well connected
    if(R<=log2(n)){
        cerr<<"R must be greater than log2(n), so that the graph is well connected"<<endl;
        return 1;
    }

    cout << endl << endl;
    cout << "Base file: " << base_file << endl;
    cout << "Query file: " <<query_file << endl;
    cout << "Groundtruth file: " << groundtruth_file << endl;
    cout << "k: " << k << endl;
    cout << "L: " << L << endl;
    cout << "R: " << R << endl;
    cout << "a: " << a << endl;
    cout << endl;
    cout << "Now the implementation of the vamana algorithm is starting!" << endl;

    // Start time measurement
    auto start = chrono::high_resolution_clock::now();

    // Run the Vamana Indexing Algorithm
    VamanaIndexingAlgorithm(nodes,k, L, R, a,n);

    cout << "Graph has been created!" <<endl;


    vector<Node*> queries = load_fvecs(query_file);
    vector<vector<int>> groundtruth = load_groundtruth(groundtruth_file);

    double totalRecall = 0.0;
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
        double recall = computeRecall(groundTruthForQuery, nearestNeighbors);
        totalRecall += recall;
        queryCount++;

        // Print the recall for this query
        cout << "Recall for query " << queryNode->id << ": " << recall << endl;
        cout << "--------------------------------------------------" << endl;
    }

    double averageRecall = totalRecall / queryCount;
    cout << "Average Recall: " << averageRecall << endl;

    // End time measurement
    auto end = chrono::high_resolution_clock::now();

    // Calculate elapsed time
    chrono::duration<double> duration = end - start;

    cout << "The vamana graph has been successfully implemented" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    //Free the allocated memory
    for(Node* node: nodes) delete node;


    return 0;
}