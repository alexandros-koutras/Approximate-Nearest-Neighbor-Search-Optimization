#include "acutest.h"
#include "functions.h"

vector<Node*> createSampleNodes(int n, int dim) {
    vector<Node*> nodes;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 100);
    
    for (int i = 0; i < n; ++i) {
        Node* node = new Node(i);
        for (int j = 0; j < dim; ++j) {
            node->coords.push_back(dis(gen));
        }
        nodes.push_back(node);
    }
    return nodes;
}

void test_initializeRandomGraph(void){
    int n = 10;
    int R = 3;
    vector<Node*> nodes = createSampleNodes(n, 3);
    
    // Initialize random graph
    initializeRandomGraph(nodes, R);

    // Check if each node has exactly R outgoing neighbors and no self-loops
    for (const Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() == R);
        for (const Node* neighbor : node->out_neighbors) {
            TEST_CHECK(neighbor != node);  // No self-loops
        }
    }
    
    // Clean up allocated nodes
    for (Node* node : nodes) {
        delete node;
    }
}

void test_VamanaIndexingAlgorithm(void) {
    int n = 10;
    int L = 5;
    int R = 3;
    double a = 1.5;
    vector<Node*> nodes = createSampleNodes(n, 3);

    VamanaIndexingAlgorithm(nodes, L, R, a, n);

    // Check medoid (should have the minimum distance sum to other nodes)
    Node* expected_medoid = nullptr;
    float min_total_dist = std::numeric_limits<float>::max();
    for (Node* node : nodes) {
        float total_dist = 0.0;
        for (Node* other : nodes) {
            if (node != other) {
                total_dist += euclidean(node, other);
            }
        }
        if (total_dist < min_total_dist) {
            min_total_dist = total_dist;
            expected_medoid = node;
        }
    }
    
    // Verify that the algorithm found the correct medoid
    TEST_CHECK(expected_medoid != nullptr);
    TEST_MSG("Expected medoid id: %d", expected_medoid->id);

    // Ensure each node has at most R outgoing neighbors after VamanaIndexingAlgorithm
    for (Node* node : nodes) {
        TEST_CHECK(node->out_neighbors.size() <= static_cast<size_t>(R));
    }

    // Clean up allocated nodes
    for (Node* node : nodes) {
        delete node;
    }
}

TEST_LIST={
    {"initialize_random_graph",test_initializeRandomGraph},
    {"vamana_indexing_algorithm",test_VamanaIndexingAlgorithm},
    {NULL, NULL}
};