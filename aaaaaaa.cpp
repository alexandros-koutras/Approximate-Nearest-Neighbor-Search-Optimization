#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cassert>
#include <fstream>
#include <algorithm>

using namespace std;

// Node structure definition
struct Node {
    int id;
    float filter;
    vector<float> coords;
    vector<Node*> out_neighbors;
};

// Function prototypes (assuming the functions are implemented as shared)
void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path);
vector<vector<float>> ReadGraph(const string& file_path);
vector<Node*> CreateGraph(const vector<vector<float>>& vectors);
vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes);

int main() {
    // Step 1: Create sample data
    vector<vector<float>> sample_vectors = {
        {0.1f, 1.0f, 2.0f, 3.0f, 1, 2},  // Node 0 with neighbors 1 and 2
        {0.2f, 4.0f, 5.0f, 6.0f, 0},     // Node 1 with neighbor 0
        {0.3f, 7.0f, 8.0f, 9.0f}         // Node 2 with no neighbors
    };

    string file_path = "test_graph.bin";

    // Step 2: Save sample data to a binary file
    SaveVectorToBinary(sample_vectors, file_path);

    // Step 3: Read the data back from the file
    vector<vector<float>> loaded_vectors = ReadGraph(file_path);

    // Verify loaded data
    cout << "Loaded Vectors:" << endl;
    for (const auto& row : loaded_vectors) {
        for (float value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    // Step 4: Create nodes and graph from the loaded vectors
    vector<Node*> nodes = CreateGraph(loaded_vectors);

    // Step 5: Convert the graph back to vectors
    vector<vector<float>> graph_vectors = createVectorFromNodes(nodes);

    // Verify the graph reconstruction
    cout << "\nGraph Vectors:" << endl;
    for (const auto& row : graph_vectors) {
        for (float value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    // Clean up dynamically allocated memory
    for (Node* node : nodes) {
        delete node;
    }

    return 0;
}
