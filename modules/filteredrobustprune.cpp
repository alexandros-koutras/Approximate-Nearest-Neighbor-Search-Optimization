#include "../include/vamana.h"

double euclidean(const Node* a, const Node* b) {
    float sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        sum += pow (a->coords[i] - b->coords[i],2);
    }
    return sqrt(sum);
}

// Compare function for 2 nodes based to their distance to another common node
bool compare_distance(Node* node1, Node* node2) {
    return node1->distance < node2->distance;
}

void FilteredRobustPrune(Node* node, vector<Node*> possible_neighbours, float a, unsigned int max_neighbours) {
    // Add the already existing neighbors to the possible neighbors
    for (Node* n_ptr : node->out_neighbors) {
        possible_neighbours.push_back(n_ptr);
    }

    for (auto it = possible_neighbours.begin(); it != possible_neighbours.end(); ) {
        if ((*it)->id == node->id) {
            it = possible_neighbours.erase(it);  // Erase and move the iterator to the next element
        } else {
            ++it;  // Only increment the iterator if no element was erased
        }
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
    while (!possible_neighbours.empty()) {
        Node* closest = possible_neighbours.front();

        // Add the node to the neighbours of the node and remove it from the possible ones
        node->out_neighbors.push_back(closest);
        possible_neighbours.erase(possible_neighbours.begin());

        if (node->out_neighbors.size() == max_neighbours) {
            break;
        }

        // Pruning method
        auto it = possible_neighbours.begin();
        while (it != possible_neighbours.end()) {
            if (node->coords.at(0) != closest->coords.at(0) && (*it)->coords.at(0) != closest->coords.at(0)) {
                continue;
            }
            float pruning = a * euclidean(closest, *it); 
            if (pruning <= (*it)->distance) {
                it = possible_neighbours.erase(it);
            } else {
                ++it;
            }
        }
    }
}