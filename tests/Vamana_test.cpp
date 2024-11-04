#include "../include/acutest.h"
#include "../include/vamana.h"

Node* create_node(unsigned int id, const vector<double>& coords) {
    Node* node = new Node();
    node->id = id;
    node->coords = coords;
    return node;
}


// Basic Vamana Indexing Functionality
void test_vamana_basic_functionality() {
    vector<Node*> nodes={
    create_node(0, {0.0, 0.0}),
    create_node(1, {1.0, 1.0}),
    create_node(2, {2.0, 2.0}),
    create_node(3, {3.0, 3.0})
    };

    // for(int i=0;i<4;i++){
    //     cout << "Node ID: " << (nodes[i] ? nodes[i]->id : -1) << " | Possible neighbors count: " << nodes[i]->out_neighbors.size() << std::endl;}
    
    unsigned int k = 1, L = 2, R = 2;
    double a = 1.5;
    int n = nodes.size();

    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);
    
    // Check if each node has at most R neighbors
    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
        //cout << "Node ID: " << node->id << " | Out neighbors: " << node->out_neighbors.size() << std::endl;
    }

    for (Node* node : nodes) delete node;
}

// Small Dataset
void test_vamana_small_dataset() {
    vector<Node*> nodes={
    create_node(0, {0.0, 0.0}),
    create_node(1, {1.0, 1.0})
    };

    // for(int i=0;i<2;i++){
    //     cout << "Node ID: " << (nodes[i] ? nodes[i]->id : -1) << " | Possible neighbors count: " << nodes[i]->out_neighbors.size() << std::endl;}

    unsigned int k = 1, L = 1, R = 1;
    double a = 1.5;
    int n = nodes.size();

    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    // Verify that each node is connected within the small dataset limit
    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
        //cout << "Node ID: " << node->id << " | Out neighbors: " << node->out_neighbors.size() << std::endl;
    }
    
    for (Node* node : nodes) delete node;
}

// Test for initializeRandomGraph
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

//Large Dataset 
void test_vamana_large_dataset() {
    const int num_nodes = 100;
    vector<Node*> nodes;
    for (int i = 0; i < num_nodes; ++i) {
        nodes.push_back(create_node(i, {static_cast<double>(i), static_cast<double>(i)}));
    }

    unsigned int k = 1, L = 10, R = 5;
    double a = 1.5;
    int n = nodes.size();

    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    // Verify that each node has at most R neighbors
    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= R);
    }

    for (Node* node : nodes) delete node;
}

// medoid calculation
void test_vamana_medoid_calculation() {
    vector<Node*> nodes = {
        create_node(0, {0.0, 0.0}), 
        create_node(1, {3.0, 4.0}),  
        create_node(2, {6.0, 8.0}),  
        create_node(3, {1.0, 1.0})   
    };                               

    int k = 1, L = 2, R = 2;
    double a = 1.5;
    int n = nodes.size();

    VamanaIndexingAlgorithm(nodes, k, L, R, a, n);

    //Expected medoid is node 1
    int medoid = nodes[0]->id;
    double min_dist = numeric_limits<double>::max();
    for (int i = 0; i < n; i++) {
        double total_dist = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                total_dist += euclidean(nodes[i], nodes[j]);
            }
        }
        if (total_dist < min_dist) {
            min_dist = total_dist;
            medoid = nodes[i]->id;
        }
    }

    TEST_CHECK(medoid == 1);

    for (Node* node : nodes) delete node;
}


TEST_LIST = {
    {"Vamana Basic Functionality", test_vamana_basic_functionality},
    {"Vamana Small Dataset", test_vamana_small_dataset},
    {"Initialize random graph", test_initializeRandomGraph},
    {"Vamana Large Dataset", test_vamana_large_dataset},
    {"Vamana Medoid Calculation", test_vamana_medoid_calculation},
    {NULL, NULL} 
};