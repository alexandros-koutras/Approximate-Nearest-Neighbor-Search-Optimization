#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "adts.h"

using namespace std;


float euclidean(const Node* a, const Node* b) {
    float sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        sum += pow (a->coords[i] - b->coords[i],2);
    }
    return sqrt(sum);
}

void RobustPrune(Node* node, vector<Node*> possible_neighbours, double a, unsigned int max_neighbours) {
    // Dereference each pointer in node->out_neighbors before inserting
    for (Node* n_ptr : node->out_neighbors) {
        possible_neighbours.push_back(n_ptr); // Keep as pointer
    }

    // Clear node's neighbors
    node->out_neighbors.clear();

    // Calculate distances to possible neighbors
    for (Node* n : possible_neighbours) {
        n->distance = euclidean(node, n);
    }

    // Sort possible neighbors by distance
    sort(possible_neighbours.begin(), possible_neighbours.end(), [](Node* lhs, Node* rhs) {
        return lhs->distance < rhs->distance;
    });

    // Select closest neighbors with pruning
    while (!possible_neighbours.empty() && node->out_neighbors.size() < max_neighbours) {
        Node* closest = possible_neighbours.front();
        node->out_neighbors.push_back(closest);
        possible_neighbours.erase(possible_neighbours.begin());

        auto it = possible_neighbours.begin();
        while (it != possible_neighbours.end()) {
            if (a * closest->distance <= (*it)->distance) {
                it = possible_neighbours.erase(it);
            } else {
                ++it;
            }
        }
    }
}




// void testRobustPrune() {
//     Node node1;
//     node1.coords = {{0.0, 0.0, 0.0}};
//     vector<Node*> neighbours = {&node1};

//     for (int i = 0; i > 100000; i++) {
//         Node* node = new Node(); // Dynamically allocate memory
//         node->coords = {{(float)i, (float)i, (float)i}};
//         neighbours.push_back(node);
//     }

//     cout << "size = " << neighbours.size() << endl;
    
//     // Call RobustPrune to test behavior
//     RobustPrune(&node1, neighbours, 1.5, 3);

//     // Free allocated memory to avoid memory leaks
//     for (Node* n : neighbours) {
//         if (n != &node1) { // Avoid deleting the statically allocated node1
//             delete n;
//         }
//     }

//     // Basic assertions to check if pruning works as expected
//     std::cout << "Test passed!" << std::endl;
// }


// int main() {
//     testRobustPrune();
//     return 0;
// }