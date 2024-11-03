#include "vamana.h"

vector<vector<int>> ivecs_read(const string& filename, int a = 1, int b = -1) {
    ifstream file(filename, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return {};
    }

    // Read the dimension of the vectors
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));

    // Calculate the size of each vector in bytes
    int vecsizeof = 1 * sizeof(int) + d * sizeof(int); // 4 bytes for dimension + d * 4 bytes for vector components

    // Move to the end of the file to find how many vectors are in the file
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int bmax = fileSize / vecsizeof;

    if (b == -1) {
        b = bmax;
    }

    if (a < 1 || b > bmax || b < a) {
        cerr << "Error: Invalid bounds" << endl;
        return {};
    }

    // Go to the position of the first vector to read
    file.seekg((a - 1) * vecsizeof, ios::beg);

    // Prepare the output vector
    vector<vector<int>> vectors(b - a + 1, vector<int>(d));

    for (int i = 0; i < b - a + 1; ++i) {
        // Read the dimension (should be the same for all vectors)
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        assert(dim == d); // Ensure all vectors have the same dimension

        // Read the vector components
        file.read(reinterpret_cast<char*>(vectors[i].data()), d * sizeof(int));
    }

    file.close();
    return vectors;
}


// Function to read .fvecs file
vector<Node*> load_vecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        //  Read dimensions
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;  
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            //  Read each coordinate
            float value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(float))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << value << endl;

        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}
