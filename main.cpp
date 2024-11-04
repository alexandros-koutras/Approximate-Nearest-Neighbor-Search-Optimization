#include "include/vamana.h"

//function to check the kind of the base file
bool ends_with(const string& str, const string& suffix) {
    if (str.size() >= suffix.size()) {
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    } else {
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        cerr << "Usage: " << argv[0] << " <base.vecs> <k> <L> <R> <a> " << endl;
        return 1;
    }

    string base_file = argv[1];
    vector<Node*> nodes;
    if (ends_with(base_file, ".ivecs")) {
        nodes = load_ivecs(base_file);
    } else if (ends_with(base_file, ".fvecs")) {
        nodes = load_fvecs(base_file);
    }else if (ends_with(base_file, ".bvecs")) {
        nodes = load_bvecs(base_file);
    }else{
        cout<<"Not a valid file"<<endl;
        return;
    }
    

    int k = stoi(argv[2]);
    int L = stoi(argv[3]);
    int R = stoi(argv[4]);
    double a = stod(argv[5]);

    int n=nodes.size();
    //well connected
    if(R<=log2(n)){
        cerr<<"R must be greater than log2(n), so that the graph is well connected"<<endl;
        return 1;
    }

    // Run the Vamana Indexing Algorithm
    VamanaIndexingAlgorithm(nodes,k, L, R, a,n);

    // Cleanup: free memory
    for (Node* node : nodes) delete node;


    return 0;
}
