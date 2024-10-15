#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

struct Node {
    int id;                                             //node's id
    vector<double> coords;               //coordinates
    vector<Node*> neighbors;           //out-neighbors
};

// Distance between two nodes (using Euclidean distance)
double calculate_distance(const Node* a, const Node* b) {
    double distance = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        distance += (a->coords[i] - b->coords[i]) * (a->coords[i] - b->coords[i]);
    }
    return sqrt(distance);
}


// GreedySearch αλγόριθμος
vector<Node*> GreedySearch(Node* s, const Node* x_q, int k, int l) {

    unordered_set<Node*> V;                                        // Visited nodes
    vector<Node*> result;                                              // k-closest nodes
    vector<Node*> L = {s};                                             // Η λίστα αναζήτησης ξεκινά με τον κόμβο s

    //uodate search list L function
    auto update_L = [&](Node* p) {

        //  travers the out-neighbors of the node
        for (Node* neighbor : p->neighbors) {
            //  if they are not visited add them to the search list L
            if (V.find(neighbor) == V.end()) {
                L.push_back(neighbor);
            }
        }

    };

    //  while search list is not empty and we have not found all the closest neighbors
    while (!L.empty() && result.size() < k) {

        //find the closest unvisited node to x_q
        Node* p_star = nullptr;
        double min_distance = numeric_limits<double>::max();

        //  traverse the search list L
        for (Node* p : L) {
            //  and calculate the distance from x_q for the nodes that are not visited
            if (V.find(p) == V.end()) {
                double distance = calculate_distance(p, x_q);
                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }

        if (p_star) {
            //  If we have found p_star, we visit it and add it to the result list
            V.insert(p_star); 
            result.push_back(p_star); 
            //  update the search list with p_star's out-neighbors
            update_L(p_star); 

            // if search list overcome l, we maintain the l closest nodes
            if (L.size() > l) {
                nth_element(L.begin(), L.begin() + l, L.end(),
                    [&](Node* a, Node* b) {
                        return calculate_distance(a, x_q) < calculate_distance(b, x_q);
                    });
                L.resize(l); // Κρατάμε μόνο τα πρώτα l στοιχεία
            }
        } else {
            // if p_star not found we stop the searching
            break; 
        }
    }

    return result; 
}



int main() {
    // Test 1: Small graph, close neighbors
    cout << "Test 1: Small graph, close neighbors\n";
    Node node1 = {1, {0.0, 0.0}};
    Node node2 = {2, {1.0, 1.0}};
    Node node3 = {3, {2.0, 2.0}};
    Node node4 = {4, {3.0, 3.0}};
    Node node5 = {5, {4.0, 4.0}};
    node1.neighbors = {&node2, &node3};
    node2.neighbors = {&node1, &node3};
    node3.neighbors = {&node2, &node4};
    node4.neighbors = {&node3, &node5};
    node5.neighbors = {&node4};

    Node x_q1 = {6, {3.5, 3.5}};
    vector<Node*> result1 = GreedySearch(&node1, &x_q1, 3, 4);

    cout << "Result for Test 1: \n";
    for (Node* node : result1) {
        cout << "Node ID: " << node->id << endl;
    }

    // Test 2: Larger, sparse graph
    cout << "\nTest 2: Larger, sparse graph\n";
    Node node6 = {6, {10.0, 10.0}};
    Node node7 = {7, {15.0, 15.0}};
    node1.neighbors = {&node2, &node3};
    node3.neighbors = {&node4};
    node4.neighbors = {&node5};
    node5.neighbors = {&node6};
    node6.neighbors = {&node7};

    Node x_q2 = {8, {12.0, 12.0}};
    vector<Node*> result2 = GreedySearch(&node1, &x_q2, 2, 3);
    cout << "Result for Test 2: \n";
    for (Node* node : result2) {
        cout << "Node ID: " << node->id << endl;
    }

    // Test 3: Far-away query point
    cout << "\nTest 3: Query point far away from nodes\n";
    Node x_q3 = {9, {100.0, 100.0}};
    vector<Node*> result3 = GreedySearch(&node1, &x_q3, 2, 3);
    cout << "Result for Test 3: \n";
    for (Node* node : result3) {
        cout << "Node ID: " << node->id << endl;
    }

    // Test 4: Tight search limit
    cout << "\nTest 4: Tight search list limit L\n";
    Node x_q4 = {10, {2.5, 2.5}};
    vector<Node*> result4 = GreedySearch(&node1, &x_q4, 2, 1); // Limit L to 1
    cout << "Result for Test 4: \n";
    for (Node* node : result4) {
        cout << "Node ID: " << node->id << endl;
    }

      // Test 3: Higher dimensions (3D coordinates)
    cout << "\nTest 3: 3D graph nodes\n";
    Node node11 = {11, {1.0, 1.0, 1.0}};
    Node node12 = {12, {2.0, 2.0, 2.0}};
    Node node13 = {13, {3.0, 3.0, 3.0}};
    Node node14 = {14, {4.0, 4.0, 4.0}};
    Node node15 = {15, {5.0, 5.0, 5.0}};
    node11.neighbors = {&node12, &node13};
    node12.neighbors = {&node11, &node13};
    node13.neighbors = {&node12, &node14};
    node14.neighbors = {&node13, &node15};
    node15.neighbors = {&node14};

    Node x_q3 = {16, {3.5, 3.5, 3.5}};
    vector<Node*> result3 = GreedySearch(&node11, &x_q3, 3, 4);
    cout << "Result for Test 3: \n";
    for (Node* node : result3) {
        cout << "Node ID: " << node->id << endl;
    }

    // Test 4: Larger, more complex graph with more neighbors
    cout << "\nTest 4: Larger graph with more neighbors\n";
    Node node16 = {16, {6.0, 6.0}};
    Node node17 = {17, {7.0, 7.0}};
    Node node18 = {18, {8.0, 8.0}};
    Node node19 = {19, {9.0, 9.0}};
    Node node20 = {20, {10.0, 10.0}};
    node15.neighbors = {&node16, &node17};
    node16.neighbors = {&node15, &node18};
    node17.neighbors = {&node18};
    node18.neighbors = {&node19};
    node19.neighbors = {&node20};
    
    Node x_q4 = {21, {9.5, 9.5}};
    vector<Node*> result4 = GreedySearch(&node11, &x_q4, 5, 6);
    cout << "Result for Test 4: \n";
    for (Node* node : result4) {
        cout << "Node ID: " << node->id << endl;
    }

    // Test 5: Query node far outside network
    cout << "\nTest 5: Far away node\n";
    Node x_q5 = {22, {100.0, 100.0}};
    vector<Node*> result5 = GreedySearch(&node1, &x_q5, 3, 5);
    cout << "Result for Test 5: \n";
    for (Node* node : result5) {
        cout << "Node ID: " << node->id << endl;
    }

    return 0;
}
