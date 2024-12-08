#include "../include/vamana.h"


void RobustPrune(Node* node, vector<Node*> possible_neighbours, double a, unsigned int max_neighbours) {
    // Add the already existing neighbors to the possible neighbors
    for (Node* n_ptr : node->out_neighbors) {
        possible_neighbours.push_back(n_ptr);
    }

    // Clear node's neighbors
    node->out_neighbors.clear();

    // Calculate distances to possible neighbors
    for (Node* n : possible_neighbours) {
        n->distance = euclidean(node, n);
    }

    // Sort possible neighbors by distance
    sort(possible_neighbours.begin(), possible_neighbours.end(), compare_distance);

    // Select closest neighbors with pruning
    // Iterate through the possible neighbors until empty or reach maxinum neighbors
    while (!possible_neighbours.empty() && node->out_neighbors.size() < max_neighbours) {
        Node* closest = possible_neighbours.front();

        // Add the node to the neighbours of the node and remove it from the possible ones
        node->out_neighbors.push_back(closest);
        possible_neighbours.erase(possible_neighbours.begin());

        // Pruning method
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