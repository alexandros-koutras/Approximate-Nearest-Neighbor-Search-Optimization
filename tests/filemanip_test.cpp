#include "../include/acutest.h"
#include "../include/vamana.h" // Include the implementation with provided functions
#include <cmath>

Node *createNode(unsigned int id, float filter, const std::vector<float> &coords, const std::vector<Node *> &neighbors) {
    Node* node = new Node();
    node->id = id;
    node->filter = filter;
    node->coords = coords;
    node->out_neighbors = neighbors;
    return node;
}

bool areVectorsEqual(const std::vector<float>& v1, const std::vector<float>& v2, float epsilon = 1e-5) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (std::abs(v1[i] - v2[i]) > epsilon) return false;
    }
    return true;
}


// Unit test for `createNodesFromVectors`
void test_createNodesFromVectors() {
    vector<vector<float>> input_vectors = {
        {1.0, 0.1, 0.5, 0.2},
        {2.0, 0.2, 1.5, 0.3},
        {1.0, 0.5, 2.5, 0.8},
        {3.0, 0.9, 0.7, 0.9}
    };

    vector<Node*> nodes = createNodesFromVectors(input_vectors);

    // Verify the number of nodes created
    TEST_CHECK(nodes.size() == 4);

    // Verify each node's `id`, `filter`, and coordinates
    TEST_CHECK(nodes[0]->id == 0);
    TEST_CHECK(nodes[0]->filter == 1.0);
    TEST_CHECK(nodes[0]->coords == vector<float>({0.5, 0.2}));

    TEST_CHECK(nodes[1]->id == 1);
    TEST_CHECK(nodes[1]->filter == 2.0);
    TEST_CHECK(nodes[1]->coords == vector<float>({1.5, 0.3}));

    TEST_CHECK(nodes[2]->id == 2);
    TEST_CHECK(nodes[2]->filter == 1.0);
    TEST_CHECK(nodes[2]->coords == vector<float>({2.5, 0.8}));

    TEST_CHECK(nodes[3]->id == 3);
    TEST_CHECK(nodes[3]->filter == 3.0);
    TEST_CHECK(nodes[3]->coords == vector<float>({0.7, 0.9}));

    // Cleanup allocated memory
    for (auto node : nodes) {
        delete node;
    }
}

void test_createQueriesFromVectors() {
    // Input vectors simulating valid and invalid queries
    std::vector<std::vector<float>> input_vectors = {
        {0, 1.0, -1, -1, 0.1, 0.2, 0.3},  // Valid query type 0
        {1, 2.0, -1, -1, 0.4, 0.5, 0.6},  // Valid query type 1
        {2, 3.0, -1, -1, 0.0, 0.0, 0.0},  // Should be skipped (type=2)
        {3, 4.0, -1, -1, 0.7, 0.8, 0.9},  // Should be skipped (type=3)
        {0, 1.5, -1, -1, 1.0, 1.1, 1.2}   // Another valid query type 0
    };

    // Call the function
    auto nodes = createQueriesFromVectors(input_vectors);

    // Verify the valid number of nodes
    TEST_CHECK(nodes.size() == 3);

    // Verify valid nodes are mapped correctly
    TEST_CHECK(nodes[0]->id == 0);
    TEST_CHECK(nodes[0]->filter == 1.0);
    TEST_CHECK(areVectorsEqual(nodes[0]->coords, {0.1, 0.2, 0.3}));

    TEST_CHECK(nodes[1]->id == 1);
    TEST_CHECK(nodes[1]->filter == 2.0);
    TEST_CHECK(areVectorsEqual(nodes[1]->coords, {0.4, 0.5, 0.6}));

    TEST_CHECK(nodes[2]->id == 4);
    TEST_CHECK(nodes[2]->filter == 1.5);
    TEST_CHECK(areVectorsEqual(nodes[2]->coords, {1.0, 1.1, 1.2}));

    // Cleanup dynamically allocated memory
    for (auto node : nodes) {
        delete node;
    }
}
// Test Suite
TEST_LIST = {
    {"Test createNodesFromVectors", test_createNodesFromVectors},
    {"Test createQueriesFromVectors", test_createQueriesFromVectors},
    {NULL, NULL}
};

