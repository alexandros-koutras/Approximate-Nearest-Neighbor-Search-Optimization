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

vector<Node*> createQueriesFromVectors(const vector<vector<float>>& vectors) {
    vector<Node*> nodes;

    for (size_t i = 0; i < vectors.size(); ++i) {
        Node* newNode = new Node;
        newNode->id = i;  // Use index as the ID
        newNode->distance = vectors[i].at(1);
        if (newNode->distance == 2 || newNode->distance == 3) {
            delete newNode;
            continue;
        } 
        newNode->filter = vectors[i].at(1);
        newNode->coords.assign(vectors[i].begin() + 2, vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}

vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes) {
    vector<vector<float>> vectors;

    for (Node* n : nodes) {
        vector<float> node;

        // Add the node ID (as an integer) and filter
        node.push_back(n->filter);                   // Filter

        // Add the coordinates to the node vector
        node.insert(node.end(), n->coords.begin(), n->coords.end());

        // Add the IDs of the neighbors
        vector<float> neighbor_ids;
        for (Node* neighbor : n->out_neighbors) {
            neighbor_ids.push_back(static_cast<float>(neighbor->id));
        }
        node.insert(node.end(), neighbor_ids.begin(), neighbor_ids.end());

        // Add this node vector to the main vectors list
        vectors.push_back(node);
    }

    return vectors;
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

/// @brief Save the vector of nodes (created by createVectorFromNodes) to a binary file
/// @param vectors The 2D vector containing nodes (each row representing a node)
/// @param file_path The path to the binary file
void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path) {
    ofstream ofs(file_path, ios::binary);
    assert(ofs.is_open());

    uint32_t N = vectors.size();
    ofs.write(reinterpret_cast<const char*>(&N), sizeof(uint32_t));  // Write number of points

    size_t num_dimensions = vectors.empty() ? 0 : vectors[0].size();
    ofs.write(reinterpret_cast<const char*>(&num_dimensions), sizeof(size_t));  // Write number of dimensions

    // Write the actual data
    for (const vector<float>& node : vectors) {
        for (float value : node) {
            ofs.write(reinterpret_cast<const char*>(&value), sizeof(float));
        }
    }

    ofs.close();
    cout << "Vector saved to " << file_path << endl;
}



vector<Node*> CreateGraph(const vector<vector<float>>& vectors) {
    vector<Node*> nodes;

    // Limit the iteration to the first 100 elements, or the size of vectors, whichever is smaller
    for (size_t i = 0; i < vectors.size(); ++i) {
        Node* newNode = new Node;
        newNode->id = i;  // Use index as the ID
        newNode->filter = vectors[i].at(0);

        // Set the limit for the number of coordinates (up to 100)
        size_t limit = std::min(vectors[i].size() - 1, size_t(100));  // Ensure not exceeding the size of the vector

        // Copy the coordinates up to the limit (starting from the 2nd element)
        newNode->coords.assign(vectors[i].begin() + 1, vectors[i].begin() + 1 + limit);

        nodes.push_back(newNode);  // Add the Node to the list
    }

    // Second loop: Set neighbors for each node
    for (size_t i = 0; i < vectors.size(); ++i) {
        for (size_t j = 101; j < vectors[i].size(); ++j) {
            float id = vectors[i].at(j);

            // Find the node with the matching id (use a lambda to compare the node id)
            auto it = std::find_if(nodes.begin(), nodes.end(), [id](Node* node) { return node->id == id; });

            // If the node with the given id exists, add it as a neighbor
            if (it != nodes.end()) {
                (*it)->out_neighbors.push_back(nodes[i]);  // Add the current node to the neighbor's out_neighbors
            }
        }
    }

    return nodes;
}

vector<vector<float>> ReadGraph(const string &file_path) {
    cout << "Reading Data: " << file_path << endl;
    ifstream ifs(file_path, ios::binary);
    assert(ifs.is_open());

    uint32_t N;  // number of points
    ifs.read((char*)&N, sizeof(uint32_t));  // Read the number of points
    cout << "# of points: " << N << endl;

    size_t num_dimensions;  // Number of dimensions
    ifs.read((char*)&num_dimensions, sizeof(size_t));  // Read the number of dimensions

    cout << "# of dimensions: " << num_dimensions << endl;

    vector<vector<float>> data(N);

    // Loop through the file and read all data points
    for (uint32_t i = 0; i < N; ++i) {
        vector<float> row(num_dimensions);  // Create a row of appropriate size
        ifs.read(reinterpret_cast<char*>(row.data()), num_dimensions * sizeof(float));  // Read a data point into the row
        data[i] = move(row);  // Store the row in the data array
    }

    ifs.close();
    cout << "Finished Reading Data" << endl;

    return data;
}
