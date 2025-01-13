#include "../include/vamana.h"
#include <thread>
#include <mutex>

void RobustPrune_Threads(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours) {
    // Add existing neighbors to possible neighbors
    for (Node* n_ptr : node->out_neighbors) {
        possible_neighbours.push_back(n_ptr);
    }

    // Remove self-loops
    for (auto it = possible_neighbours.begin(); it != possible_neighbours.end();) {
        if ((*it)->id == node->id) {
            it = possible_neighbours.erase(it);
        } else {
            ++it;
        }
    }

    // Clear node's neighbors
    node->out_neighbors.clear();

    mutex dist_mutex; // Mutex for thread-safe access to distances

    // Parallel computation of distances
    vector<thread> threads;
    size_t num_threads = thread::hardware_concurrency();
    size_t chunk_size = possible_neighbours.size() / num_threads;

    for (size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([&, t]() {
            size_t start = t * chunk_size;
            size_t end = (t == num_threads - 1) ? possible_neighbours.size() : start + chunk_size;

            for (size_t i = start; i < end; ++i) {
                Node* n = possible_neighbours[i];
                lock_guard<mutex> lock(dist_mutex);
                n->distance = euclidean(node, n);
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    // Sort possible neighbors by distance
    sort(possible_neighbours.begin(), possible_neighbours.end(), compare_distance);

    // Select closest neighbors with pruning
    while (!possible_neighbours.empty()) {
        Node* closest = possible_neighbours.front();

        // Add the closest node to the neighbors of the node
        node->out_neighbors.push_back(closest);
        possible_neighbours.erase(possible_neighbours.begin());

        if (node->out_neighbors.size() == static_cast<size_t>(max_neighbours)) {
            break;
        }

        // Pruning method
        auto it = possible_neighbours.begin();
        while (it != possible_neighbours.end()) {
            float pruning = a * euclidean(closest, *it);
            if (pruning <= (*it)->distance) {
                it = possible_neighbours.erase(it);
            } else {
                ++it;
            }
        }
    }
}
