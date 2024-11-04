#include "../include/acutest.h"

#include "../include/vamana.h"

// Helper function to create a Node with specified coordinates
Node* create_node(unsigned int id, const vector<double>& coords) {
    Node* node = new Node();
    node->id = id;
    node->coords = coords;
    return node;
}

void test_euclidean_distance() {
    Node* node1 = create_node(1, {1.0, 2.0, 3.0});
    Node* node2 = create_node(2, {4.0, 5.0, 6.0});
    double expected_distance = sqrt(27.0);  // sqrt((4-1)^2 + (5-2)^2 + (6-3)^2)
    TEST_CHECK(euclidean(node1, node2) == expected_distance);
}

void test_node_add_get_neighbour() {
    Node* node1 = create_node(1, {1.0, 1.0, 1.0});
    Node* node2 = create_node(2, {2.0, 2.0, 2.0});
    node1->out_neighbors.push_back(node2);

    TEST_CHECK(node1->out_neighbors.size() == 1);
    TEST_CHECK(node1->out_neighbors.at(0)->coords == node2->coords);
}

void test_robust_prune() {
    Node* central_node = create_node(10, {0.0, 0.0, 0.0});
    
    Node* node1 = create_node(1, {1.0, 0.0, 0.0});
    Node* node2 = create_node(2, {0.0, 1.0, 0.0});
    Node* node3 = create_node(3, {0.0, 0.0, 1.0});
    Node* node4 = create_node(4, {2.0, 2.0, 2.0});

    vector<Node*> possible_neighbours = { node1, node2, node3, node4 };

    
    unsigned int max_neighbours = 2;
    double a = 1.5;

    RobustPrune(central_node, possible_neighbours, a, max_neighbours);

    // Check that central node has the correct number of neighbors after pruning
    TEST_CHECK(central_node->out_neighbors.size() == max_neighbours);
    // Check distances to ensure pruning logic is correctly applied
    if (!central_node->out_neighbors.empty()) {
        double closest_distance = central_node->out_neighbors.front()->distance;
        for (Node* neighbor : central_node->out_neighbors) {
            TEST_CHECK(neighbor->distance <= a * closest_distance);
        }
    }
}

// Register tests with Acutest
TEST_LIST = {
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_node_add_get_neighbour", test_node_add_get_neighbour},
    {"test_robust_prune", test_robust_prune},

    {NULL, NULL} // Terminate the list
};
