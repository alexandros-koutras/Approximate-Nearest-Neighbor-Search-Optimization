#include "vamana.h"
#include "acutest.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cassert>

using namespace std;


vector<vector<float>> ReadGraph(const string &file_path) {
    cout << "Reading Data: " << file_path << endl;
    ifstream ifs(file_path, ios::binary);
    assert(ifs.is_open());

    uint32_t N;  // number of points
    ifs.read((char*)&N, sizeof(uint32_t));  // Read the number of points
    cout << "# of points: " << N << endl;

    vector<vector<float>> data(N);

    // Loop through the file and read all data points
    for (uint32_t i = 0; i < N; ++i) {
        size_t num_dimensions;  // Dimensions for the current point
        ifs.read((char*)&num_dimensions, sizeof(size_t));  // Read dimensions for this point

        cout << "Point " << i << " dimensions: " << num_dimensions << endl;

        vector<float> row(num_dimensions);  // Create a row with appropriate size
        ifs.read(reinterpret_cast<char*>(row.data()), num_dimensions * sizeof(float));  // Read data into the row
        data[i] = move(row);  // Store the row in the data array
    }

    ifs.close();
    cout << "Finished Reading Data" << endl;

    return data;
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
        coords.insert(coords.end(), vf.begin() + 1, vf.begin() + 9);
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
            for (size_t j = 9; j < vf.size(); j++) {
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

void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path) {
    ofstream ofs(file_path, ios::binary);
    assert(ofs.is_open());

    uint32_t N = vectors.size();
    ofs.write(reinterpret_cast<const char*>(&N), sizeof(uint32_t));  // Write number of points

    // Write the actual data for each vector
    for (const vector<float>& node : vectors) {
        size_t num_dimensions = node.size();  // Get the number of dimensions for this particular vector
        ofs.write(reinterpret_cast<const char*>(&num_dimensions), sizeof(size_t));  // Write number of dimensions for this vector

        // Write the values of this vector
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

void test_read_and_create_graph() {
    vector<vector<float>> test_data = {
        {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9},
        {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9},
        {2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9},
        {3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9},
        {4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9},
        {5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9},
        {6.0, 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8, 6.9},
        {7.0, 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9},
        {8.0, 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9},
        {9.0, 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9}
    };

    vector<Node*> nodes = createNodesFromVectors(test_data);

    nodes[0]->out_neighbors.push_back(nodes[1]);
    nodes[0]->out_neighbors.push_back(nodes[5]);
    nodes[0]->out_neighbors.push_back(nodes[2]);
    nodes[0]->out_neighbors.push_back(nodes[3]);
    nodes[0]->out_neighbors.push_back(nodes[7]);
    nodes[1]->out_neighbors.push_back(nodes[3]);
    nodes[1]->out_neighbors.push_back(nodes[4]);
    nodes[1]->out_neighbors.push_back(nodes[2]);
    nodes[1]->out_neighbors.push_back(nodes[6]);
    nodes[2]->out_neighbors.push_back(nodes[5]);
    nodes[2]->out_neighbors.push_back(nodes[8]);
    nodes[2]->out_neighbors.push_back(nodes[9]);
    nodes[3]->out_neighbors.push_back(nodes[6]);
    nodes[3]->out_neighbors.push_back(nodes[4]);
    nodes[3]->out_neighbors.push_back(nodes[5]);
    nodes[4]->out_neighbors.push_back(nodes[7]);
    nodes[4]->out_neighbors.push_back(nodes[2]);
    nodes[4]->out_neighbors.push_back(nodes[0]);
    nodes[5]->out_neighbors.push_back(nodes[8]);
    nodes[5]->out_neighbors.push_back(nodes[1]);
    nodes[5]->out_neighbors.push_back(nodes[6]);
    nodes[6]->out_neighbors.push_back(nodes[9]);
    nodes[6]->out_neighbors.push_back(nodes[2]);
    nodes[6]->out_neighbors.push_back(nodes[8]);
    nodes[7]->out_neighbors.push_back(nodes[0]);
    nodes[7]->out_neighbors.push_back(nodes[3]);
    nodes[7]->out_neighbors.push_back(nodes[9]);
    nodes[8]->out_neighbors.push_back(nodes[0]);
    nodes[8]->out_neighbors.push_back(nodes[5]);
    nodes[9]->out_neighbors.push_back(nodes[4]);
    nodes[9]->out_neighbors.push_back(nodes[6]);

    vector<vector<float>> back_to_vecs = createVectorFromNodes(nodes);

    SaveVectorToBinary(back_to_vecs, "graph.bin");

    cout << "Okay\n";

    vector<vector<float>> graph_vecs = ReadGraph("graph.bin");

    vector<Node*> graph = CreateGraph(graph_vecs);

    for (int i = 0; i < 5; i++) {
        cout << graph[0]->out_neighbors[i]->id << endl;
    }

    cout << endl;

    for (int i = 0; i < 4; i++) {
        cout << graph[1]->out_neighbors[i]->id << endl;
    }

    cout << endl;

    for (int i = 0; i < 3; i++) {
        cout << graph[2]->out_neighbors[i]->id << endl;
    }

    cout << endl;

    for (int i = 0; i < 3; i++) {
        cout << graph[3]->out_neighbors[i]->id << endl;
    }

    cout << endl;

    TEST_ASSERT(graph[0]->id == 0);
    std::vector<float> expected_coords0 = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    TEST_ASSERT(graph[0]->coords == expected_coords0);
    TEST_ASSERT(graph[0]->filter == 0.0);

    TEST_ASSERT(graph[1]->id == 1);
    std::vector<float> expected_coords1 = {1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9};
    TEST_ASSERT(graph[1]->coords == expected_coords1);
    TEST_ASSERT(graph[1]->filter == 1.0);

    TEST_ASSERT(graph[2]->id == 2);
    std::vector<float> expected_coords2 = {2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9};
    TEST_ASSERT(graph[2]->coords == expected_coords2);
    TEST_ASSERT(graph[2]->filter == 2.0);

    TEST_ASSERT(graph[3]->id == 3);
    std::vector<float> expected_coords3 = {3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9};
    TEST_ASSERT(graph[3]->coords == expected_coords3);
    TEST_ASSERT(graph[3]->filter == 3.0);

    TEST_ASSERT(graph[4]->id == 4);
    std::vector<float> expected_coords4 = {4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9};
    TEST_ASSERT(graph[4]->coords == expected_coords4);
    TEST_ASSERT(graph[4]->filter == 4.0);

    TEST_ASSERT(graph[5]->id == 5);
    std::vector<float> expected_coords5 = {5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9};
    TEST_ASSERT(graph[5]->coords == expected_coords5);
    TEST_ASSERT(graph[5]->filter == 5.0);

    TEST_ASSERT(graph[6]->id == 6);
    std::vector<float> expected_coords6 = {6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8, 6.9};
    TEST_ASSERT(graph[6]->coords == expected_coords6);
    TEST_ASSERT(graph[6]->filter == 6.0);
}

TEST_LIST = {
    {"test_read_and_create_graph", test_read_and_create_graph},

    {NULL, NULL}
};