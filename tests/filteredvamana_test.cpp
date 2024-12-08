#include "../include/acutest.h"
#include "../include/vamana.h"



// Helper: Create a Node
Node* createNode(unsigned int id, const vector<float>& coords, const unordered_set<string>& tags) {
    Node* node = new Node();
    node->id = id;
    node->coords =coords;
    node->tags = tags;
    return node;
}

// Test Fisher-Yates Shuffle
void test_fisher_yates_shuffle() {
    vector<Node*> nodes = {
        createNode(0, {0.0, 0.0}),
        createNode(1, {1.0, 1.0}),
        createNode(2, {2.0, 2.0}),
        createNode(3, {3.0, 3.0})
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

// Test Medoid Calculation
void test_find_medoid() {
    vector<Node*> nodes = {
        createNode(0, {0.0, 0.0}),
        createNode(1, {3.0, 4.0}),
        createNode(2, {6.0, 8.0}),
        createNode(3, {1.0, 1.0})
    };

    int k = 1;  // Clustering parameter
    Node* medoid = findMedoid(nodes, k);

    // Calculate expected medoid manually (minimal total distance)
    int expected_medoid_id = 0;  // Manually computed for this dataset
    double min_dist = numeric_limits<double>::max();

    for (Node* candidate : nodes) {
        double total_dist = 0;
        for (Node* other : nodes) {
            if (candidate != other) {
                total_dist += euclidean(candidate, other);
            }
        }
        if (total_dist < min_dist) {
            min_dist = total_dist;
            expected_medoid_id = candidate->id;
        }
    }

    TEST_CHECK(medoid->id == expected_medoid_id);

    for (Node* node : nodes) delete node;
}

// Test Filtered Vamana for Edge Cases
void test_filtered_vamana() {
    // Small Dataset
    vector<Node*> nodes = {
        createNode(0, {0.0, 0.0}),
        createNode(1, {1.0, 1.0}),
        createNode(2, {2.0, 2.0})
    };

    unsigned int k = 1, L = 2, R = 1;
    float alpha = 1.5;

    DirectedGraph graph = FilteredVamana(nodes, k, L, R, alpha);

    // Test adjacency list size
    for (auto& [node, neighbors] : graph.adjacency_list) {
        TEST_CHECK(neighbors.size() <= R);  // Check pruning
    }

    for (Node* node : nodes) delete node;

    // Large Dataset
    const int num_nodes = 100;
    vector<Node*> large_nodes;
    for (int i = 0; i < num_nodes; ++i) {
        large_nodes.push_back(createNode(i, {static_cast<float>(i), static_cast<float>(i)}));
    }

    DirectedGraph large_graph = FilteredVamana(large_nodes, k, L, R, alpha);

    // Test adjacency list size in large dataset
    for (auto& [node, neighbors] : large_graph.adjacency_list) {
        TEST_CHECK(neighbors.size() <= R);
    }

    for (Node* node : large_nodes) delete node;

    //Single Node
    vector<Node*> single_node = {createNode(0, {0.0, 0.0})};
    DirectedGraph single_graph = FilteredVamana(single_node, k, L, R, alpha);

    TEST_CHECK(single_graph.adjacency_list.size() == 1);  // Single node graph
    TEST_CHECK(single_graph.adjacency_list[single_node[0]].empty());  // No neighbors

    delete single_node[0];
}

void test_filtered_vamana_with_tags() {
    vector<Node*> nodes = {
        createNode(0, {0.0, 0.0}, {"A"}),
        createNode(1, {1.0, 1.0}, {"A", "B"}),
        createNode(2, {2.0, 2.0}, {"B"}),
        createNode(3, {3.0, 3.0}, {"C"})
    };

    unsigned int k = 1, L = 2, R = 1;
    float alpha = 1.5;

    DirectedGraph graph = FilteredVamana(nodes, k, L, R, alpha);

    // Ensure that neighbors respect tag-based filtering
    for (Node* node : nodes) {
        for (Node* neighbor : graph.adjacency_list[node]) {
            TEST_CHECK(!node->tags.empty());
            bool sharedTag = false;
            for (const string& tag : node->tags) {
                if (neighbor->tags.count(tag)) {
                    sharedTag = true;
                    break;
                }
            }
            TEST_CHECK(sharedTag);  // Ensure shared tag exists
        }
    }

    for (Node* node : nodes) delete node;
}

TEST_LIST = {
    {"Fisher-Yates Shuffle", test_fisher_yates_shuffle},
    {"Find Medoid", test_find_medoid},
    {"Filtered Vamana Edge Cases", test_filtered_vamana},
    {"Filtered Vamana with tags", test_filtered_vamana_with_tags},
    {NULL, NULL}
};
