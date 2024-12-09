#include "../include/acutest.h"

#include "../include/vamana.h"

// Helper function to create a Node with specified coordinates
Node* create_node(unsigned int id, const vector<float>& coords) {
    Node* node = new Node();
    node->id = id;
    node->coords = coords;
    return node;
}

// Test that the euclidean works as expected
void test_euclidean_distance() {
    Node* node1 = create_node(1, {1.0, 2.0, 3.0});
    Node* node2 = create_node(2, {4.0, 5.0, 6.0});
    float expected_distance = sqrt(27.0);  // sqrt((4-1)^2 + (5-2)^2 + (6-3)^2)
    TEST_CHECK(euclidean(node1, node2) == expected_distance);

    delete node1;
    delete node2;
}

void test_node_add_get_neighbour() {
    Node* node1 = create_node(1, {1.0, 1.0, 1.0});
    Node* node2 = create_node(2, {2.0, 2.0, 2.0});
    node1->out_neighbors.push_back(node2);

    TEST_CHECK(node1->out_neighbors.size() == 1);
    TEST_CHECK(node1->out_neighbors.at(0)->coords == node2->coords);

    delete node1;
    delete node2;
}

void test_compare_func_for_nodes() {
    Node* reference_node = create_node(0, {0.0, 0.0});
    Node* node1 = create_node(1, {1.0, 1.0});
    Node* node2 = create_node(2, {2.0, 2.0});

    node1->distance = euclidean(reference_node, node1);
    node2->distance = euclidean(reference_node, node2);

    bool d = compare_distance(node1, node2);
    TEST_CHECK(d == true);

    delete reference_node;
    delete node1;
    delete node2;
}

void test_robust_prune() {
    // Create central node and neighbors
    Node* central_node = create_node(10, {0.0, 0.0, 0.0});
    Node* node1 = create_node(1, {1.0, 0.0, 0.0});
    Node* node2 = create_node(2, {0.0, 1.0, 0.0});
    Node* node3 = create_node(3, {0.0, 0.0, 1.0});
    Node* node4 = create_node(4, {2.0, 2.0, 2.0});

    vector<Node*> possible_neighbours = { node1, node2, node3, node4 };

    // Define parameters
    int max_neighbours = 2;
    float a = 1.5;

    // Run RobustPrune
    RobustPrune(central_node, possible_neighbours, a, max_neighbours);

    // Check if the correct number of neighbors were selected
    TEST_CHECK(central_node->out_neighbors.size() <= static_cast<size_t>(max_neighbours));

    // Validate that the closest neighbors are chosen
    vector<float> distances;
    for (Node* neighbor : central_node->out_neighbors) {
        distances.push_back(neighbor->distance);
    }
    
    // Check if distances are sorted and within the acceptable range
    float closest_distance = distances.front();
    bool in_range = true;
    for (float dist : distances) {
        TEST_CHECK(dist <= a * closest_distance);
        in_range = in_range && (dist <= a * closest_distance);
    }
    
    // Ensure distances are sorted
    bool sorted = is_sorted(distances.begin(), distances.end());
    TEST_CHECK(sorted);

    delete central_node;
    delete node1;
    delete node2;
    delete node3;
    delete node4;
}

void test_robust_prune_with_filters() {
    // Create central node and neighbors with different filters
    Node* central_node = create_node(10, {0.0, 0.0, 0.0});
    central_node->filter = 1.0;

    Node* node1 = create_node(1, {1.0, 0.0, 0.0});
    node1->filter = 1.0;  // Same filter as central node
    Node* node2 = create_node(2, {0.0, 1.0, 0.0});
    node2->filter = 1.0;  // Same filter as central node
    Node* node3 = create_node(3, {0.0, 0.0, 1.0});
    node3->filter = 2.0;  // Different filter
    Node* node4 = create_node(4, {2.0, 2.0, 2.0});
    node4->filter = 3.0;  // Different filter

    vector<Node*> possible_neighbours = { node1, node2, node3, node4 };

    // Define parameters
    int max_neighbours = 2;
    float a = 1.5;

    // Run RobustPrune
    RobustPrune(central_node, possible_neighbours, a, max_neighbours);

    // Check if only nodes with matching filters were selected
    TEST_CHECK(central_node->out_neighbors.size() <= static_cast<size_t>(max_neighbours));
    for (Node* neighbor : central_node->out_neighbors) {
        TEST_CHECK(neighbor->filter == central_node->filter);
    }

    delete central_node;
    delete node1;
    delete node2;
    delete node3;
    delete node4;
}

// void test_robust_prune_edge_cases_with_filters() {
//     // Test case: All neighbors have different filters
//     Node* central_node = create_node(10, {0.0, 0.0, 0.0});
//     central_node->filter = 1.0;

//     Node* node1 = create_node(1, {1.0, 0.0, 0.0});
//     node1->filter = 2.0;  // Different filter
//     Node* node2 = create_node(2, {0.0, 1.0, 0.0});
//     node2->filter = 3.0;  // Different filter
//     Node* node3 = create_node(3, {0.0, 0.0, 1.0});
//     node3->filter = 4.0;  // Different filter

//     vector<Node*> possible_neighbours = { node1, node2, node3 };

//     // Define parameters
//     int max_neighbours = 2;
//     float a = 1.5;

//     // Run RobustPrune
//     RobustPrune(central_node, possible_neighbours, a, max_neighbours);

//     // No neighbors should be selected since none have matching filters
//     TEST_CHECK(central_node->out_neighbors.empty());

//     delete central_node;
//     delete node1;
//     delete node2;
//     delete node3;
// }

// void test_robust_prune_mixed_filters() {
//     // Central node with matching and non-matching neighbors
//     Node* central_node = create_node(10, {0.0, 0.0, 0.0});
//     central_node->filter = 1.0;

//     Node* node1 = create_node(1, {1.0, 0.0, 0.0});
//     node1->filter = 1.0;  // Same filter
//     Node* node2 = create_node(2, {0.0, 1.0, 0.0});
//     node2->filter = 2.0;  // Different filter
//     Node* node3 = create_node(3, {0.0, 0.0, 1.0});
//     node3->filter = 1.0;  // Same filter

//     vector<Node*> possible_neighbours = { node1, node2, node3 };

//     // Define parameters
//     int max_neighbours = 2;
//     float a = 1.5;

//     // Run RobustPrune
//     RobustPrune(central_node, possible_neighbours, a, max_neighbours);

//     // Ensure only nodes with matching filters are selected
//     TEST_CHECK(central_node->out_neighbors.size() == 2);
//     for (Node* neighbor : central_node->out_neighbors) {
//         TEST_CHECK(neighbor->filter == central_node->filter);
//     }

//     delete central_node;
//     delete node1;
//     delete node2;
//     delete node3;
// }



// Register tests with Acutest
TEST_LIST = {
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_node_add_get_neighbour", test_node_add_get_neighbour},
    {"test_compare_func_for_nodes", test_compare_func_for_nodes},
    {"test_robust_prune", test_robust_prune},
    {"test_robust_prune_with_filters", test_robust_prune_with_filters},

    {NULL, NULL} // Terminate the list
};