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
    const float alpha = 1.2f;  // Παράμετρος κλαδέματος
    const int L_small = 20;
    const int R_small = 10;
    const int R_stitched = 15;

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

    Node* node5 = create_node(5, {1.0, 0.0, 0.0});
    node1->filter = 1.0;  // Same filter as central node
    Node* node6 = create_node(6, {0.0, 1.0, 0.0});
    node2->filter = 1.0;  // Same filter as central node
    Node* node7 = create_node(7, {0.0, 0.0, 1.0});
    node3->filter = 2.0;  // Different filter
    Node* node8 = create_node(8, {2.0, 2.0, 2.0});
    node4->filter = 3.0;  // Different filter

    vector<Node*> nodes = { node1, node2, node3, node4, node5, node6, node7, node8};

    // Εκτέλεση της Stitched Vamana
    StitchedVamana(nodes, alpha, L_small, R_small, R_stitched);

    // Επαλήθευση Ιδιότητων του Τελικού Γράφου
    for (Node* n : nodes) {
        // Βεβαιώνουμε ότι κανένας κόμβος δεν έχει περισσότερους από R_stitched γείτονες
        assert(n->out_neighbors.size() <= static_cast<size_t>(R_stitched));

        // Βεβαιώνουμε ότι όλοι οι γείτονες έχουν κοινά φίλτρα με τον τρέχοντα κόμβο
        for (Node* neighbor : n->out_neighbors) {
            assert(n->filter == neighbor->filter);
        }
    }

    cout << "All tests passed for StitchedVamana!" << endl;

    // Διαγραφή κόμβων για αποφυγή διαρροής μνήμης
    for (Node* n : nodes) {
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