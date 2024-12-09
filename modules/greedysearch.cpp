#include "../include/vamana.h"


// GreedySearch αλγόριθμος
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) {
    if (!s) {
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;  // Visited nodes
    unordered_set<Node*> unique_nodes; // Ensure unique nodes in the result
    vector<Node*> L = {s};   // Start with the initial node in the search list

    // Priority queue for efficiently finding the closest unvisited node
    using NodeDistPair = pair<double, Node*>;
    priority_queue<NodeDistPair, vector<NodeDistPair>, greater<>> pq;

    // Prepopulate the priority queue with the starting node
    pq.emplace(euclidean(s, x_q), s);

    while (any_of(L.begin(), L.end(), [&](Node* p) { return V.find(p) == V.end(); })) {
        // Find the closest unvisited node
        auto top = pq.top();
        Node* p_star = top.second;
        pq.pop();

        // Skip if already visited
        if (V.find(p_star) != V.end()) {
            continue;
        }

        // Mark the node as visited
        V.insert(p_star);

        // Add out-neighbors of `p_star` to `L` if not visited
        for (Node* neighbor : p_star->out_neighbors) {
            if (V.find(neighbor) == V.end() && unique_nodes.find(neighbor) == unique_nodes.end()) {
                L.push_back(neighbor);
                unique_nodes.insert(neighbor); // Mark as unique
                pq.emplace(euclidean(neighbor, x_q), neighbor); // Add to priority queue
            }
        }

        // If L exceeds the allowed size, retain only the closest `list_size` points
        if (L.size() > list_size) {
            nth_element(L.begin(), L.begin() + list_size, L.end(),
                        [&](Node* a, Node* b) {
                            return euclidean(a, x_q) < euclidean(b, x_q);
                        });
            L.resize(list_size);
        }
    }

    // Extract the closest `k` nodes from L
    if (L.size() > k) {
        nth_element(L.begin(), L.begin() + k, L.end(),
                    [&](Node* a, Node* b) {
                        return euclidean(a, x_q) < euclidean(b, x_q);
                    });
        L.resize(k);
    }

    return L; // Return the `k` closest unique points from `L`
}
