#include "vamana.h"

void RobustPrune(Node* node, vector<Node*> possible_neighbours, double a, int max_neighbours) {
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
    std::sort(possible_neighbours.begin(), possible_neighbours.end(), [](Node* lhs, Node* rhs) {
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
