//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Map.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////
#include "greedysearch.cpp"
#include "acutest/include/acutest.h"// Acutest library
#include <iostream>
using namespace std;

Node* create_node(unsigned int id, const vector<double>& coords) {
    Node* node = new Node();
    node->id = id;
    node->coords = coords;
    return node;
}


// Test 1: Basic Functionality Test
void test_basic_functionality() {
    Node* node0 = create_node(0, {0.0, 0.0});
    Node* node1 = create_node(1, {1.0, 1.0});
    Node* node2 = create_node(2, {2.0, 2.0});
    node0->neighbors = {node1, node2};

    Node query;
    query.coords = {0.5, 0.5};
    vector<Node*> result = GreedySearch(node0, &query, 1, 3);
    TEST_CHECK(result.size() == 1);
    TEST_CHECK(result[0]->id == 0);

    delete node0;
    delete node1;
    delete node2;
}

// Test 2: Empty Graph Test
void test_empty_graph() {
    Node query;
    query.coords = {0.5, 0.5};
    vector<Node*> result = GreedySearch(nullptr, &query, 1, 1);
    TEST_CHECK(result.empty());
}


// Test 3: Single Node Graph Test
void test_single_node() {
    Node* node0 = create_node(0, {1.0, 1.0});
    Node query;
    query.coords = {1.0, 1.0};
    vector<Node*> result = GreedySearch(node0, &query, 1, 1);
    TEST_CHECK(result.size() == 1);
    TEST_CHECK(result[0]->id == 0);
    
    delete node0;
}


void test_multiple_nodes_one_query() {
    Node node1, node2, query;
    node1.coords = {1.0, 1.0};
    node2.coords = {2.0, 2.0};
    query.coords = {1.5, 1.5};
    node1.neighbors.push_back(&node2); // Node1 has Node2 as neighbor
    vector<Node*> result = GreedySearch(&node1, &query, 1, 1);
    TEST_CHECK(result.size() == 1 && result[0] == &node1); // Node1 should be closest
}

// List of tests
TEST_LIST = {
    {"Basic Functionality", test_basic_functionality},
    {"Empty Graph", test_empty_graph},
    {"Single Node Graph", test_single_node},
    {"Multiple Nodes one query", test_multiple_nodes_one_query},
    {NULL, NULL} // End of the list
};
