#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <filename> <L> <R> <a>" << endl;
        return 1;
    }

    string filename = argv[1];
    int L = stoi(argv[2]);
    int R = stoi(argv[3]);
    double a = stod(argv[4]);

    // Load the dataset as nodes
    vector<Node*> nodes = load_vecs(filename);

    // Run the Vamana Indexing Algorithm
    VamanaIndexingAlgorithm(nodes, L, R, a);

    // Display results or save the graph structure, as needed
    for (Node* node : nodes) {
        cout << "Node " << node->id << ": ";
        for (Node* neighbor : node->out_neighbors) {
            cout << neighbor->id << " ";
        }
        cout << endl;
    }

    // Clean up dynamically allocated nodes
    for (Node* node : nodes) {
        delete node;
    }

    return 0;
}
