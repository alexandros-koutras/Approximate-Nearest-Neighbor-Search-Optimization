#include "../include/vamana.h"


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

vector<vector<float>> ReadGroundTruth(const string& file_path) {
    cout << "Reading Ground Truth: " << file_path << endl;
    ifstream ifs(file_path, ios::binary);
    assert(ifs.is_open());

    uint32_t N;
    ifs.read(reinterpret_cast<char*>(&N), sizeof(uint32_t));  // Number of points
    size_t num_dimensions;
    ifs.read(reinterpret_cast<char*>(&num_dimensions), sizeof(size_t));  // Number of dimensions

    vector<vector<float>> data(N, vector<float>(num_dimensions));
    for (auto& row : data) {
        ifs.read(reinterpret_cast<char*>(row.data()), num_dimensions * sizeof(float));
    }

    ifs.close();
    return data;
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
        newNode->distance = vectors[i].at(0);
        if (newNode->distance == 2 || newNode->distance == 3) {
            delete newNode;
            continue;
        } 
        newNode->filter = vectors[i].at(1);
        newNode->coords.assign(vectors[i].begin() + 4, vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}

vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes) {
    vector<vector<float>> vectors;

    for (Node* n : nodes) {
        vector<float> node;

        node.push_back(n->filter);

        node.insert(node.end(), n->coords.begin(), n->coords.end());

        vector<float> neighbor_ids;
        for (Node* neighbor : n->out_neighbors) {
            neighbor_ids.push_back(static_cast<float>(neighbor->id));
        }
        node.insert(node.end(), neighbor_ids.begin(), neighbor_ids.end());

        vectors.push_back(node);
    }

    return vectors;
}

vector<Node*> CreateGraph(vector<vector<float>> vectors) {
    int i = 0;
    vector<Node*> nodes;
    for (vector<float> vf : vectors) {
        Node* newNode = new Node;

        newNode->id = i;
        i++;

        newNode->filter = vf.at(0);
        
        vector<float> coords;
        coords.insert(coords.end(), vf.begin() + 1, vf.begin() + 100);
        newNode->coords = coords;

        nodes.push_back(newNode);
    }

    int a = 0;
    for (vector<float> vf : vectors) {
        int check = 0;
        for (Node* n : nodes) {
            if (check != a) {
                check++;
                continue;
            }
            for (size_t j = 101; j < vf.size(); j++) {
                Node* neighbor;
                unsigned int findID = vf.at(j);
                for (Node* n : nodes) {
                    if (n->id == findID) {
                        neighbor = n;
                        break;
                    }
                }
                n->out_neighbors.push_back(neighbor);
            }
            break;
        }
        a++;
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
