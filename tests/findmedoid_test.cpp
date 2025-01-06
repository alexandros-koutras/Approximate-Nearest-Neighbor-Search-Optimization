#include "../include/acutest.h"
#include "../include/vamana.h" // Include the updated implementation

// Helper function to create a Node
Node* createNode(unsigned int id, float filter, const vector<float>& coords) {
    Node* node = new Node();
    node->id = id;
    node->filter = filter; // Set the explicit filter
    node->coords = coords;
    return node;
}

void test_medoid_basic() {
    // Create test nodes with explicit filters
    vector<Node*> nodes = {
        createNode(0, 1.0, {0.5, 0.2}),
        createNode(1, 2.0, {1.5, 0.3}),
        createNode(2, 1.0, {2.5, 0.8}),
        createNode(3, 3.0, {0.7, 0.9})
    };

    // Number of random samples
    unsigned int tau = 2;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau);

    // Verify results
    TEST_CHECK(medoids.find(1.0) != medoids.end()); // Filter 1.0 should have a medoid
    TEST_CHECK(medoids.find(2.0) != medoids.end()); // Filter 2.0 should have a medoid
    TEST_MSG("Medoid ID for filter 1.0: %d", medoids[1.0]);
    TEST_MSG("Medoid ID for filter 2.0: %d", medoids[2.0]);

    // Cleanup
    for (auto node : nodes) {
        delete node;
    }
}

void test_medoid_empty_filter() {
    // Create test nodes with explicit filters
    vector<Node*> nodes = {
        createNode(0, 1.0, {0.5, 0.2}),
        createNode(1, 2.0, {1.5, 0.3})
    };

    // Number of random samples
    unsigned int tau = 1;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau);

    // Verify results
    TEST_CHECK(medoids.find(3.0) == medoids.end()); // No medoid should exist for filter 3.0

    // Cleanup
    for (auto node : nodes) {
        delete node;
    }
}

void test_medoid_small_tau() {
    // Create test nodes with explicit filters
    vector<Node*> nodes = {
        createNode(0, 1.0, {0.5, 0.2}),
        createNode(1, 1.0, {1.5, 0.3}),
        createNode(2, 1.0, {2.5, 0.8})
    };

    // Small tau value
    unsigned int tau = 1;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau);

    // Verify results
    TEST_CHECK(medoids.find(1.0) != medoids.end());
    TEST_MSG("Medoid ID for filter 1.0: %d", medoids[1.0]);

    // Cleanup
    for (auto node : nodes) {
        delete node;
    }
}

// Test Suite
TEST_LIST = {
    {"Basic Medoid Test", test_medoid_basic},
    {"Empty Filter Test", test_medoid_empty_filter},
    {"Small Tau Test", test_medoid_small_tau},
    {NULL, NULL}
};
