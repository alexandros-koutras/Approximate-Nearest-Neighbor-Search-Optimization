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
        newNode->filter = vectors[i].at(0);
        newNode->coords.assign(vectors[i].begin() + 2, vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}

/// @brief Reading binary data vectors. Raw data store as a (N x dim)
/// @param file_path file path of binary data
/// @param data returned 2D data vectors
vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions) {
    cout << "Reading Data: " << file_path << endl;
    ifstream ifs;
    ifs.open(file_path, ios::binary);
    assert(ifs.is_open());
    uint32_t N;  // num of points
    ifs.read((char *)&N, sizeof(uint32_t));
    std::vector<std::vector<float>> data(N);
    cout << "# of points: " << N << endl;
    vector<float> buff(num_dimensions);
    int counter = 0;
    while (ifs.read((char *)buff.data(), num_dimensions * sizeof(float))) {
        vector<float> row(num_dimensions);
        for (int d = 0; d < num_dimensions; d++) {
            row[d] = static_cast<float>(buff[d]);
        }
        data[counter++] = move(row);
    }
    ifs.close();
    cout << "Finish Reading Data" << endl;

    return data;
}