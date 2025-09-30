#include "../include/vamana.h"

mutex distance_mutex; // Definition here

void calculate_distances_parallel(const Node* x_q, const vector<Node*>& nodes, unordered_map<Node*, double>& distances) {
   vector<thread> threads;
    size_t num_threads =thread::hardware_concurrency();
    size_t chunk_size = nodes.size() / num_threads;

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, i]() {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? nodes.size() : (i + 1) * chunk_size;

            for (size_t j = start; j < end; ++j) {
                double dist = euclidean(nodes[j], x_q);
               lock_guard<mutex> lock(distance_mutex);
                distances[nodes[j]] = dist;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Parallel GreedySearch
vector<Node*> GreedySearchaaaa(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) {
    if (!s) return {};

    unordered_set<Node*> V;
    vector<Node*> L = {s};
    unordered_map<Node*, double> distances;

    calculate_distances_parallel(x_q, L, distances);

    while (any_of(L.begin(), L.end(), [&](Node* p){ return V.find(p) == V.end(); })) {
        Node* p_star = nullptr;
        double min_distance = numeric_limits<double>::max();

        for (Node* p : L) {
            if (V.find(p) == V.end()) {
                double distance = distances[p];
                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }

        if (p_star) {
            V.insert(p_star);
            for (Node* neighbor : p_star->out_neighbors) {
                if (V.find(neighbor) == V.end()) {
                    L.push_back(neighbor);
                }
            }

            if (L.size() > list_size) {
                calculate_distances_parallel(x_q, L, distances);
                nth_element(L.begin(), L.begin() + list_size, L.end(), [&](Node* a, Node* b) {
                    return distances[a] < distances[b];
                });
                L.resize(list_size);
            }
        } else {
            break;
        }
    }

    unordered_set<Node*> unique_nodes(L.begin(), L.end());
    L.assign(unique_nodes.begin(), unique_nodes.end());

    if (L.size() > k) {
        nth_element(L.begin(), L.begin() + k, L.end(), [&](Node* a, Node* b) {
            return distances[a] < distances[b];
        });
        L.resize(k);
    }

    return L;
}
