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
        cerr << "Usage: " << argv[0] << " -i <base.vecs> -k <k> -l <L> -r <R> -a <a>\n";
        return 1;
    }

    string base_file;
    int k = 0, L = 0, R = 0;
    double a = 0.0;

    int opt;
    while ((opt = getopt(argc, argv, "i:k:l:r:a:")) != -1) {
        switch (opt) {
            case 'i':
                base_file = optarg;
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

    // End time measurement
    auto end = chrono::high_resolution_clock::now();

    // Calculate elapsed time
    chrono::duration<double> duration = end - start;

    cout << "The vamana graph has been successfully implemented" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    // Cleanup: free memory
    for (Node* node : nodes) delete node;


    return 0;
}