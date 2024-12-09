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

void testStitchedVamana() {
    const float alpha = 1.2f;
    const int L_small = 15;
    const int R_small = 10;
    const int R_stitched = 12;

    // Generate 100 nodes with random coordinates
    const int num_nodes = 100;
    vector<Node*> nodes;
    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < num_nodes; ++i) {
        vector<float> coords = {static_cast<float>(rand() % 100) / 10.0f,
                                static_cast<float>(rand() % 100) / 10.0f,
                                static_cast<float>(rand() % 100) / 10.0f};
        Node* node = create_node(i, coords);
        node->filter = static_cast<float>(rand() % 3 + 1);
        nodes.push_back(node);
    }

    cout << "Generated " << num_nodes << " nodes with random positions and filters." << endl;

    // Run the Stitched Vamana algorithm
    try {
        StitchedVamana(nodes, alpha, L_small, R_small, R_stitched);
    } catch (const std::exception& e) {
        cerr << "Error during StitchedVamana execution: " << e.what() << endl;
        assert(false);
    }

    // Verify properties of the final graph
    for (Node* n : nodes) {
        assert(n->out_neighbors.size() <= static_cast<size_t>(R_stitched));
        for (Node* neighbor : n->out_neighbors) {
            assert(n->filter == neighbor->filter);
        }
    }

    cout << "All tests passed for StitchedVamana with " << num_nodes << " nodes!" << endl;

    // Cleanup
    for (Node* n : nodes) {
        n->out_neighbors.clear(); // Avoid dangling references
        delete n;
    }
}






// Register tests with Acutest
TEST_LIST = {
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_node_add_get_neighbour", test_node_add_get_neighbour},
    {"test_compare_func_for_nodes", test_compare_func_for_nodes},
    {"testStitchedVamana", testStitchedVamana},

    {NULL, NULL} // Terminate the list
};