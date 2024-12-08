#include "../includes/acutest.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "findmedoid.h" // Include the updated implementation

using namespace std;

void test_medoid_basic() {
    // Create test nodes
    vector<Node*> nodes = {
        new Node{0, 0.0, {1.0, 0.5, 0.2}}, // First coord is 1.0
        new Node{1, 0.0, {2.0, 1.5, 0.3}}, // First coord is 2.0
        new Node{2, 0.0, {1.0, 2.5, 0.8}}, // First coord is 1.0
        new Node{3, 0.0, {3.0, 0.7, 0.9}}  // First coord is 3.0
    };

    // Define filters (first coordinate values)
    unordered_set<float> filters = {1.0, 2.0};

    // Number of random samples
    unsigned int tau = 2;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau, filters);

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
    // Create test nodes
    vector<Node*> nodes = {
        new Node{0, 0.0, {1.0, 0.5, 0.2}}, // First coord is 1.0
        new Node{1, 0.0, {2.0, 1.5, 0.3}}  // First coord is 2.0
    };

    // Define a filter not matching any node
    unordered_set<float> filters = {3.0}; // No nodes with first coord 3.0

    // Number of random samples
    unsigned int tau = 1;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau, filters);

    // Verify results
    TEST_CHECK(medoids.find(3.0) == medoids.end()); // No medoid should exist for filter 3.0

    // Cleanup
    for (auto node : nodes) {
        delete node;
    }
}

void test_medoid_small_tau() {
    // Create test nodes
    vector<Node*> nodes = {
        new Node{0, 0.0, {1.0, 0.5, 0.2}},
        new Node{1, 0.0, {1.0, 1.5, 0.3}},
        new Node{2, 0.0, {1.0, 2.5, 0.8}}
    };

    // Define filters
    unordered_set<float> filters = {1.0};

    // Small tau value
    unsigned int tau = 1;

    // Run findmedoid
    unordered_map<float, unsigned int> medoids = findmedoid(nodes, tau, filters);

    // Verify results
    TEST_CHECK(medoids.find(1.0) != medoids.end());
    TEST_MSG("Medoid ID for filter 1.0: %d", medoids[1.0]);

    // Cleanup
    for (auto node : nodes) {
        delete node;
    }
}

TEST_LIST = {
    {"Basic Medoid Test", test_medoid_basic},
    {"Empty Filter Test", test_medoid_empty_filter},
    {"Small Tau Test", test_medoid_small_tau},
    {NULL, NULL}
};
