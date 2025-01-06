#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cassert>

using namespace std;


struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors; 
    float filter;
};

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

// This is the euklideian method to calculate the distance of 2 nodes
float euclidean(const Node* a, const Node* b) {
    float sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        float diff = a->coords[i] - b->coords[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Compare function for 2 nodes based to their distance to another common node
bool compare_distance(Node* node1, Node* node2) {
    return node1->distance < node2->distance;
}

vector<vector<float>> brute_force(vector<Node*>& nodes, vector<Node*>& queries) {
    vector<vector<float>> groundtruth(5012);
    int i = 0;

    for (Node* query : queries) {
        for (Node* node : nodes) {
            node->distance = euclidean(query, node);
        }

        sort(nodes.begin(), nodes.end(), compare_distance);

        vector<float> k_closest;
        int k = 0;
        for (Node* node : nodes) {
            if (k == 100) {
                break;
            }
            k_closest.push_back(static_cast<float>(node->id));
            k++;
        }

        groundtruth[i] = k_closest;

        i++;
    }

    return groundtruth;
}

void print_vectors(vector<float> coords) {
    for (float c : coords) {
        cout << c << " ";
    }
}

void print_groundtruth(vector<vector<float>> groundtruth) {
    for (size_t i = 0; i < groundtruth.size(); i++) {
        cout << "Node id " << i << endl;
        print_vectors(groundtruth.at(i));
        cout << endl << endl;
    }
}

int main() {
    vector<vector<float>> data = ReadBin("dummy-data.bin", 102);
    vector<Node*> nodes = createNodesFromVectors(data);

    vector<vector<float>> queries_vectors = ReadBin("dummy-queries.bin", 104);
    vector<Node*> queries = createQueriesFromVectors(queries_vectors);

    vector<vector<float>> groundtruth = brute_force(nodes, queries);

    SaveVectorToBinary(groundtruth, "dummy-groundtruth.bin");

    print_groundtruth(groundtruth);

    cout << endl;
}