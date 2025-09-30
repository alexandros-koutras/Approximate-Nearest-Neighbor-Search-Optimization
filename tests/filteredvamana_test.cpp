#include "../include/acutest.h"
#include "../include/vamana.h"

// Helper: Create a Node
Node* createNode(unsigned int id, float filter, const vector<float>& coords, const vector<Node*>& neighbors) {
    Node* node = new Node();
    node->id = id;
    node->filter = filter;  
    node->coords = coords;
    node->out_neighbors = neighbors;
    return node;
}

// Test Fisher-Yates Shuffle
void test_fisher_yates_shuffle() {
    vector<Node*> nodes = {
        createNode(0, 0.0, {0.0, 0.0}, {}),
        createNode(1, 1.0, {1.0, 1.0}, {}),
        createNode(2, 2.0, {2.0, 2.0}, {}),
        createNode(3, 1.0, {3.0, 3.0}, {})
    };

    fisherYatesShuffle(nodes);

    // Test if all nodes are present after shuffling
    unordered_set<unsigned int> ids;
    for (Node* node : nodes) {
        ids.insert(node->id);
    }
    TEST_CHECK(ids.size() == 4);  // No duplicates
    TEST_CHECK(ids.find(0) != ids.end()); 
    TEST_CHECK(ids.find(1) != ids.end());
    TEST_CHECK(ids.find(2) != ids.end());
    TEST_CHECK(ids.find(3) != ids.end());

    for (Node* node : nodes) delete node;
}
//tests for filtered vamana

//Test1: Small dataset
void test_small_dataset_distinct_filters() {
    Node* node0 = createNode(0, 0.0, {0.0, 0.0}, {});
    Node* node1 = createNode(1, 1.0, {0.0, 0.0}, {});
    Node* node2 = createNode(2, 2.0, {1.0, 1.0}, {});
    Node* node3 = createNode(3, 3.0, {2.0, 2.0}, {});
    vector<Node*> databasePoints = {node0,node1, node2, node3};
    int k = 1;
    unsigned int L = 2;
    unsigned int R = 2;
    float alpha = 0.5;
    unsigned int tau = 2;
    
    //FilteredVamana
    DirectedGraph G = FilteredVamana(databasePoints, k, L, R, alpha, tau);
    
    //graph properties
    TEST_CHECK(G.adjacency_list.size() == databasePoints.size()); // All nodes should be in the graph
    for (Node* node : databasePoints) {
        TEST_CHECK(G.adjacency_list[node].size() <= R); // Out-degree <= R
    }
    
    // Cleanup
    for (Node* node : databasePoints) {
        delete node;
    }
}

// Test2: Empty dataset
void test_empty_dataset() {
    vector<Node*> databasePoints;
    int k = 1;
    unsigned int L = 2;
    unsigned int R = 2;
    float alpha = 0.5;
    unsigned int tau = 1;

    //FilteredVamana
    DirectedGraph G = FilteredVamana(databasePoints, k, L, R, alpha, tau);

    // Assert empty graph
    TEST_CHECK(G.adjacency_list.size() == 0);
}

// Test3: Single node
void test_single_node() {
    Node* node = createNode(0, 0.0, {0.0, 0.0}, {});
    vector<Node*> databasePoints = {node};
    int k = 1;
    unsigned int L = 1;
    unsigned int R = 1;
    float alpha = 0.5;
    unsigned int tau = 1;

    //FilteredVamana
    DirectedGraph G = FilteredVamana(databasePoints, k, L, R, alpha, tau);

    // Assert graph single-node
    TEST_CHECK(G.adjacency_list.size() == 1);
    TEST_CHECK(G.adjacency_list[node].empty()); // Single node has no neighbors

    delete node;
}

// Test4: All nodes same filter
void test_same_filter() {
    Node* node0=createNode(0, 0.0, {0.0, 0.0}, {});
    Node* node1 = createNode(1, 1.0, {0.0, 0.0}, {});
    Node* node2 = createNode(2, 1.0, {1.0, 1.0}, {});
    vector<Node*> databasePoints = {node0,node1, node2};
    int k = 1;
    unsigned int L = 2;
    unsigned int R = 2;
    float alpha = 0.5;
    unsigned int tau = 1;

    //FilteredVamana
    DirectedGraph G = FilteredVamana(databasePoints, k, L, R, alpha, tau);

    //all nodes are connected since they share the same filter
    TEST_CHECK(G.adjacency_list.size() == databasePoints.size());
    for (Node* node : databasePoints) {
        TEST_CHECK(G.adjacency_list[node].size() <= R); // Out-degree <= R
    }

    // Cleanup
    for (Node* node : databasePoints) {
        delete node;
    }
}

// Test5: Large dataset
void test_large_dataset() {
    vector<Node*> databasePoints;
    for (unsigned int i = 0; i < 1000; ++i) {
        databasePoints.push_back(createNode(i, i % 10, {static_cast<float>(i), 0.0}, {}));
    }
    int k = 5;
    unsigned int L = 10;
    unsigned int R = 15;
    float alpha = 0.7;
    unsigned int tau = 10;

    //FilteredVamana
    DirectedGraph G = FilteredVamana(databasePoints, k, L, R, alpha, tau);

    // Assert basic properties
    TEST_CHECK(G.adjacency_list.size() == databasePoints.size());
    for (Node* node : databasePoints) {
        TEST_CHECK(G.adjacency_list[node].size() <= R); // Out-degree <= R
    }

    // Cleanup
    for (Node* node : databasePoints) {
        delete node;
    }
}

TEST_LIST = {
    {"Fisher-Yates Shuffle", test_fisher_yates_shuffle},
    {"Small dataset with distinct filters", test_small_dataset_distinct_filters},
    {"Empty dataset", test_empty_dataset},
    {"Single node dataset", test_single_node},
    {"All nodes have the same filter", test_same_filter},
    // {"Large dataset with varying filters", test_large_dataset},
    {NULL, NULL}
};