#include "vamana.h"
#include "acutest.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cassert>

using namespace std;


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
    if (node1->distance == node2->distance) {
        return node1->id < node2->id; // Secondary criterion: sort by ID
    }
    return node1->distance < node2->distance;
}

vector<vector<float>> brute_force(vector<Node*>& nodes, vector<Node*>& queries) {
    vector<vector<float>> groundtruth(5012);
    int i = 0;

    for (Node* query : queries) {
        if (query->distance == 0) {
            for (Node* node : nodes) {
                node->distance = euclidean(query, node);
            }

            sort(nodes.begin(), nodes.end(), compare_distance);

            vector<float> k_closest;
            int k = 0;
            for (Node* node : nodes) {
                if (k == 10) {
                    break;
                }
                k_closest.push_back(static_cast<float>(node->id));
                k++;
            }

            groundtruth[i] = k_closest;

            i++;
        } else {
            vector<Node*> new_nodes;
            for (Node* node : nodes) {
                if (node->filter == query->filter) {
                    node->distance = euclidean(query, node);
                    new_nodes.push_back(node);
                }
            }

            sort(new_nodes.begin(), new_nodes.end(), compare_distance);

            vector<float> k_closest;
            int k = 0;
            for (Node* node : new_nodes) {
                if (k == 10) {
                    break;
                }
                k_closest.push_back(static_cast<float>(node->id));
                k++;
            }

            groundtruth[i] = k_closest;

            i++;
        }
    }

    return groundtruth;
}

void print_vectors(vector<float> coords) {
    for (float c : coords) {
        cout << c << " ";
    }
}

void print_groundtruth(vector<vector<float>> groundtruth, vector<Node*> queries) {
    for (size_t i = 0; i < queries.size(); i++) {
        Node* query = queries[i];
        vector<float> ground = groundtruth[i];
        cout << "Node id " << query->id << endl;
        print_vectors(groundtruth.at(i));
        cout << endl << endl;
    }
}

void test_brute_force_basic_case() {
    // Prepare nodes
    vector<vector<float>> node_data = {
        {1.0, 0.0, 0.0, 0.0, 0.0},  // Node 0
        {1.0, 1.0, 1.0, 1.0, 1.0},  // Node 1
        {2.0, 2.0, 2.0, 2.0, 2.0},  // Node 2
        {0.0, 0.0, 3.0, 3.0, 3.0},  // Node 3
        {1.0, 1.0, 4.0, 4.0, 4.0},  // Node 4
        {1.0, 5.0, 5.0, 5.0, 5.0},  // Node 8
        {1.0, 6.0, 6.0, 6.0, 6.0},  // Node 9
        {0.0, 7.0, 7.0, 7.0, 7.0},  // Node 10
        {1.0, 8.0, 8.0, 8.0, 8.0},  // Node 11
        {1.0, 9.0, 9.0, 9.0, 9.0},  // Node 12
        {0.0, 10.0, 10.0, 10.0, 10.0},  // Node 13
        {0.0, 11.0, 11.0, 11.0, 11.0},  // Node 14
        {1.0, 12.0, 12.0, 12.0, 12.0},  // Node 15
        {1.0, 13.0, 13.0, 13.0, 13.0},  // Node 16
        {2.0, 14.0, 14.0, 14.0, 14.0},  // Node 17
        {0.0, 15.0, 15.0, 15.0, 15.0},  // Node 18
        {1.0, 16.0, 16.0, 16.0, 16.0},  // Node 19
        {1.0, 17.0, 17.0, 17.0, 17.0},  // Node 20
        {2.0, 18.0, 18.0, 18.0, 18.0},  // Node 21
        {0.0, 19.0, 19.0, 19.0, 19.0},  // Node 22
        {1.0, 20.0, 20.0, 20.0, 20.0},  // Node 23
        {1.0, 21.0, 21.0, 21.0, 21.0},  // Node 24
        {2.0, 22.0, 22.0, 22.0, 22.0},  // Node 25
        {0.0, 23.0, 23.0, 23.0, 23.0},  // Node 26
        {1.0, 24.0, 24.0, 24.0, 24.0},  // Node 27
        {1.0, 25.0, 25.0, 25.0, 25.0},  // Node 28
        {2.0, 26.0, 26.0, 26.0, 26.0},  // Node 29
        {0.0, 27.0, 27.0, 27.0, 27.0},  // Node 30
        {1.0, 28.0, 28.0, 28.0, 28.0},  // Node 31
        {1.0, 29.0, 29.0, 29.0, 29.0},  // Node 32
        {2.0, 30.0, 30.0, 30.0, 30.0},  // Node 33
        {0.0, 31.0, 31.0, 31.0, 31.0},  // Node 34
        {1.0, 32.0, 32.0, 32.0, 32.0},  // Node 35
        {1.0, 33.0, 33.0, 33.0, 33.0},  // Node 36
        {2.0, 34.0, 34.0, 34.0, 34.0},  // Node 37
        {0.0, 35.0, 35.0, 35.0, 35.0},  // Node 38
        {1.0, 36.0, 36.0, 36.0, 36.0},  // Node 39
        {1.0, 37.0, 37.0, 37.0, 37.0},  // Node 40
        {2.0, 38.0, 38.0, 38.0, 38.0},  // Node 41
        {0.0, 39.0, 39.0, 39.0, 39.0},  // Node 42
        {1.0, 40.0, 40.0, 40.0, 40.0},  // Node 43
        {1.0, 41.0, 41.0, 41.0, 41.0},  // Node 44
        {2.0, 42.0, 42.0, 42.0, 42.0},  // Node 45
        {0.0, 43.0, 43.0, 43.0, 43.0},  // Node 46
        {1.0, 44.0, 44.0, 44.0, 44.0},  // Node 47
        {1.0, 45.0, 45.0, 45.0, 45.0},  // Node 48
        {1.0, 46.0, 46.0, 46.0, 46.0},  // Node 49
        {0.0, 47.0, 47.0, 47.0, 47.0},  // Node 50
        // Repeat similar pattern until Node 99
    };
    vector<Node*> nodes = createNodesFromVectors(node_data);

    // Prepare queries
    vector<vector<float>> query_data = {
        {0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0},  // Query 0
        {1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0},  // Query 1
        {0.0, -1.0, 15.0, 15.0, 15.0, 15.0, 15.0}, // Query 3
        {1.0, 1.0, 3.0, 3.0, 3.0, 3.0, 3.0} // Query 2
    };
    vector<Node*> queries = createQueriesFromVectors(query_data);

    // Perform brute-force search
    vector<vector<float>> groundtruth = brute_force(nodes, queries);

    // Validate results
    // For Query 0 (distance == 0), closest to all nodes, sorted by ID
    TEST_CHECK(groundtruth[0].size() == 10);
    TEST_CHECK(groundtruth[0][0] == 0);  // Closest
    TEST_CHECK(groundtruth[0][1] == 1);
    TEST_CHECK(groundtruth[0][2] == 2);
    TEST_CHECK(groundtruth[0][3] == 3);
    TEST_CHECK(groundtruth[0][4] == 4);
    TEST_CHECK(groundtruth[0][5] == 5);
    TEST_CHECK(groundtruth[0][6] == 6);
    TEST_CHECK(groundtruth[0][7] == 7);
    TEST_CHECK(groundtruth[0][8] == 8);
    TEST_CHECK(groundtruth[0][9] == 9);  // Furthest

    // For Query 1 (distance != 0, filter == 1.0), only Node 2 matches
    TEST_CHECK(groundtruth[1].size() == 10);
    TEST_CHECK(groundtruth[1][0] == 1);  // Check that Node 2 is the closest
    TEST_CHECK(groundtruth[1][1] == 0);
    TEST_CHECK(groundtruth[1][2] == 4);
    TEST_CHECK(groundtruth[1][3] == 5);
    TEST_CHECK(groundtruth[1][4] == 6);
    TEST_CHECK(groundtruth[1][5] == 8);
    TEST_CHECK(groundtruth[1][6] == 9);
    TEST_CHECK(groundtruth[1][7] == 12);
    TEST_CHECK(groundtruth[1][8] == 13);
    TEST_CHECK(groundtruth[1][9] == 16);

    TEST_CHECK(groundtruth[2].size() == 10);
    TEST_CHECK(groundtruth[2][0] == 15);  // Check that Node 2 is the closest
    TEST_CHECK(groundtruth[2][1] == 14);
    TEST_CHECK(groundtruth[2][2] == 16);
    TEST_CHECK(groundtruth[2][3] == 13);
    TEST_CHECK(groundtruth[2][4] == 17);
    TEST_CHECK(groundtruth[2][5] == 12);
    TEST_CHECK(groundtruth[2][6] == 18);
    TEST_CHECK(groundtruth[2][7] == 11);
    TEST_CHECK(groundtruth[2][8] == 19);
    TEST_CHECK(groundtruth[2][9] == 10);

    // Clean up
    for (Node* node : nodes) delete node;
    for (Node* query : queries) delete query;
}

TEST_LIST = {
    {"test_brute_force_basic_case", test_brute_force_basic_case},

    {NULL, NULL}
};
