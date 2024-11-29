#include "../include/vamana.h"

// Function to read .ivecs file
vector<vector<int>> loadIvecs(const string& filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<vector<int>> data;

    while (true) {
        int numDims;
        infile.read(reinterpret_cast<char*>(&numDims), sizeof(int));
        if (infile.eof()) break;

        vector<int> vec(numDims);
        infile.read(reinterpret_cast<char*>(vec.data()), numDims * sizeof(int));

        data.push_back(vec);
    }

    infile.close();
    return data;
}

// Function to read .fvecs file
vector<vector<float>> loadFvecs(const string& filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<vector<float>> data;

    while (true) {
        unsigned int numDims; // First 4 bytes specify the number of dimensions
        infile.read(reinterpret_cast<char*>(&numDims), sizeof(int));
        if (infile.eof()) break;

        vector<float> vec(numDims); // Read the `float` vector
        infile.read(reinterpret_cast<char*>(vec.data()), numDims * sizeof(float));

        if (infile.gcount() != static_cast<std::streamsize>(numDims * sizeof(float))) {
            cerr << "Error reading vector data. Incomplete vector found." << endl;
            break;
        }


        data.push_back(vec);
    }

    infile.close();
    return data;
}

// Function to read .bvecs file
vector<Node*> load_bvecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            unsigned char value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(unsigned char))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            //cout << "Value " << i << ": " << static_cast<int>(value) << endl;
        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}

vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors) {
    vector<Node*> nodes;

    for (size_t i = 0; i < vectors.size(); ++i) {
        Node* newNode = new Node;
        newNode->id = i;  // Use index as the ID
        newNode->coords.assign(vectors[i].begin(), vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}