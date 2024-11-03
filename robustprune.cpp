#include "vamana.h"

void RobustPrune(Node* node, vector<Node*>& possible_neighbours, double a, int max_neighbours) {
    for (auto it = possible_neighbours.begin(); it != possible_neighbours.end(); ) {
        if (max_neighbours == 0) {
            break;
        }
        Node* possible_vertex = *it;
        double dist = euclidean(node, possible_vertex);
        if (dist >= a) {
            it = possible_neighbours.erase(it);
        } else {
            ++it;
        }
    }

    // Prune excess neighbors if we have more than max_neighbours
    while (possible_neighbours.size() > static_cast<size_t>(max_neighbours)) {
        possible_neighbours.pop_back();
    }

    for (Node* neighbor : possible_neighbours) {
        node->add_neighbour(neighbor);
    }
}