#include "../include/acutest.h"
#include "../include/vamana.h"

// Helper: Create a Node
Node* create_node(unsigned int id, const vector<float>& coords = {}, const unordered_set<string>& tags = {}) {
    Node* node = new Node();
    node->id = id;
    node->coords = coords;
    node->tags = tags;
    return node;
}

// Test 1: Random Graph Initialization
void test_initializeRandomGraph() {
    const int num_nodes = 10;
    const int R = 3;
    
    vector<Node> node_storing(num_nodes);
    vector<Node*> nodes;
    for (int i = 0; i < num_nodes; ++i) {
        node_storing[i].id = i;
        nodes.push_back(&node_storing[i]);
    }
    
    initializeRandomGraph(nodes, R);
    
    // Test: Each node has exactly R neighbors, no duplicates and not itself
    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() == R);

        unordered_set<int> unique_neighbors;
        for (Node* neighbor : node->out_neighbors) {
            TEST_CHECK(neighbor->id != node->id);
            TEST_CHECK(unique_neighbors.find(neighbor->id) == unique_neighbors.end());
            unique_neighbors.insert(neighbor->id);
        }
    }
}


// Test 2: Test Vamana Indexing
void test_vamana() {
    // Small dataset
    vector<Node*> nodes = {
        create_node(0, {0.0, 0.0}),
        create_node(1, {1.0, 1.0}),
        create_node(2, {2.0, 2.0}),
        create_node(3, {3.0, 3.0})
    };

    unsigned int k = 1, L = 2, R = 2;
    double a = 1.5;
    int n = nodes.size();

    // Test Vamana Indexing Algorithm
    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
    }

    for (Node* node : nodes) delete node;

    // Test with a minimal dataset
    nodes = {
        create_node(0, {0.0, 0.0}),
        create_node(1, {1.0, 1.0})
    };

    k = 1, L = 1, R = 1;
    a = 1.5;
    n = nodes.size();
    
    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
    }

    for (Node* node : nodes) delete node;

    // Large Dataset Test
    const int large_num_nodes = 100;
    nodes.clear();
    for (int i = 0; i < large_num_nodes; ++i) {
        nodes.push_back(create_node(i, {static_cast<float>(i), static_cast<float>(i)}));
    }

    k = 1;
    L = 10;
    R = 5;
    a = 1.5;
    n = nodes.size();

    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
    }

    for (Node* node : nodes) delete node;
}


void test_no_nodes() {
    vector<Node*> nodes = {};
    DirectedGraph G = StitchedVamana(nodes, 1.0, 2, 2, 3);
    assert(G.adjacency_list.empty());
    cout << "Test no nodes passed." << endl;
}

// void test_no_tags() {
//     vector<Node*> nodes = {create_node(0,{0.0, 0.0}, {" "}), create_node(1,{1.0, 1.0}, {" "})};
//     DirectedGraph G = StitchedVamana(nodes, 1.0, 2, 2, 3);
//     assert(G.adjacency_list.empty());
//     cout << "Test nodes without tags passed." << endl;
// }

void test_single_unique_tag() {
    vector<Node*> nodes = {
        create_node(0,{0.0, 0.0}, {"tag1"}),
        create_node(1,{1.0, 1.0}, {"tag1"}),
        create_node(2, {2.0, 2.0}, {"tag1"})
    };
    DirectedGraph G = StitchedVamana(nodes, 1.0, 2, 2, 3);
    assert(G.adjacency_list.size() == 3);
    cout << "Test single unique tag passed." << endl;
}

void test_multiple_unique_tags() {
    vector<Node*> nodes = {
        create_node(0,{0.0, 0.0}, {"tag1"}),
        create_node(1,{1.0, 1.0}, {"tag1"}),
        create_node(2, {2.0, 2.0}, {"tag2"}),
        create_node(3, {3.0, 3.0}, {"tag2"})
    };
    DirectedGraph G = StitchedVamana(nodes, 1.0, 2, 2, 3);
    assert(G.adjacency_list.size() == 4);
    cout << "Test multiple unique tags passed." << endl;
}

// void test_filtered_prune() {
//     vector<Node*> nodes = {
//         create_node(0,{0.0, 0.0}, {"tag1"}),
//         create_node(1,{1.0, 1.0}, {"tag1"})
//     };

//     // Mock out_neighbors
//     nodes[0]->out_neighbors = {nodes[1]};
//     nodes[1]->out_neighbors = {nodes[0]};

//     DirectedGraph G = StitchedVamana(nodes, 0.5, 2, 2, 2);
//     assert(G.adjacency_list[nodes[0]].size() <= 2);
//     assert(G.adjacency_list[nodes[1]].size() <= 2);

//     cout << "Test filtered prune passed." << endl;
// }

// Register Tests
TEST_LIST = {
    {"Random Graph Initialization", test_initializeRandomGraph},
    {"Vamana Indexing Algorithm", test_vamana},
    {"Stitched Vamana no nodes", test_no_nodes},
    // {"Stitched Vamana no tags", test_no_tags},
    {"Stitched Vamana single tag", test_single_unique_tag},
    {"Stitched Vamana multiple unique tags", test_multiple_unique_tags},
    // {"Stitched Vamana filtered prune", test_filtered_prune},
    {NULL, NULL}
};