#include "../includes/filteredgreedysearch.h"
#include "../includes/acutest.h"
using namespace std;

// Helper function to create a Node
Node* createNode(unsigned int id, float filter, const vector<float>& coords, const vector<Node*>& neighbors) {
    Node* node = new Node();
    node->id = id;
    node->filter = filter;  // Assign filter attribute
    node->coords = coords;
    node->out_neighbors = neighbors;
    return node;
}

// Test Cases
void test_small_dataset_single_filter() {
    // Create nodes
    Node* node1 = createNode(1, 1.0, {2.0, 3.0}, {});
    Node* node2 = createNode(2, 3.0, {2.0, 1.0}, {});
    Node* node3 = createNode(3, 5.0, {5.0, 5.0}, {});
    Node* node4 = createNode(4, 1.0, {3.0, 5.0}, {});

    // Define neighbors
    node1->out_neighbors = {node2, node3};
    node2->out_neighbors = {node4};
    node3->out_neighbors = {};
    node4->out_neighbors = {};

    // Define query node
    Node query;
    query.filter = 0.0;  // Not used for querying neighbors
    query.coords = {3.0, 3.0, 3.0};

    // Define the query filter (only nodes with filter attribute in {1.0})
    unordered_set<float> query_filter = {1.0};

    // Perform search
    vector<Node*> start_nodes = {node1, node2, node3, node4};
    vector<Node*> result = FilteredGreedySearch(start_nodes, &query, 2, 5, query_filter);

    // Sort the result nodes by distance to the query node
    sort(result.begin(), result.end(), [&](Node* a, Node* b) {
        return euclidean(a, &query) < euclidean(b, &query);
    });

    // Add debug output to check which nodes were selected
    cout << "Filtered and sorted result nodes (IDs): ";
    for (Node* node : result) {
        cout << node->id << " ";
    }
    cout << endl;

    // Assert results
    TEST_CHECK(result.size() == 2);  // Expecting 2 nodes to match the filter
    TEST_CHECK(result[0]->id == 1);  // Node 1 should match the filter (filter = 1.0)
    TEST_CHECK(result[1]->id == 4);  // Node 4 should also match the filter (filter = 1.0)

    // Cleanup
    delete node1;
    delete node2;
    delete node3;
    delete node4;
}


void test_resilience_invalid_inputs() {
    // Empty graph
    vector<Node*> start_nodes;
    Node query;
    query.coords = {3.0, 3.0, 3.0};
    query.filter = 0.0;
    unordered_set<float> query_filter = {1.0, 3.0};

    vector<Node*> result = FilteredGreedySearch(start_nodes, &query, 3, 5, query_filter);

    TEST_CHECK(result.empty()); // No nodes to process

    // Null query node
    result = FilteredGreedySearch(start_nodes, nullptr, 3, 5, query_filter);

    TEST_CHECK(result.empty()); // No query node provided
}

void test_large_dataset() {
    // Create a large graph
    vector<Node*> nodes;
    for (unsigned int i = 0; i < 100; ++i) {
        nodes.push_back(createNode(i, static_cast<float>(i), {1.0, 2.0}, {}));
    }

    // Link the nodes in a linear fashion
    for (unsigned int i = 0; i < 99; ++i) {
        nodes[i]->out_neighbors.push_back(nodes[i + 1]);
    }

    // Define query node
    Node query;
    query.coords = {50.0, 1.0, 2.0};
    query.filter = 0.0;  // Not used for querying neighbors

    // Define query filter (only even IDs as floats)
    unordered_set<float> query_filter;
    for (unsigned int i = 0; i < 100; i += 2) {
        query_filter.insert(static_cast<float>(i));
    }

    // Perform search
    vector<Node*> result = FilteredGreedySearch(nodes, &query, 5, 10, query_filter);

    // Assert results
    TEST_CHECK(result.size() == 5);
    for (Node* node : result) {
        TEST_CHECK(static_cast<int>(node->filter) % 2 == 0); // All results must satisfy the filter
    }

    // Cleanup
    for (Node* node : nodes) {
        delete node;
    }
}

// Test Suite
TEST_LIST = {
    {"Small dataset with single filter", test_small_dataset_single_filter},
    {"Resilience to invalid inputs", test_resilience_invalid_inputs},
    {"Large dataset performance", test_large_dataset},
    {nullptr, nullptr} // Termination of the test list
};
