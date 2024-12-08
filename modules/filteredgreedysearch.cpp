#include "../includes/filteredgreedysearch.h"



// FilteredGreedySearch function
vector<Node*> FilteredGreedySearch(vector<Node*> S, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& filters_q) {
    unordered_set<Node*> V;  // Visited nodes
    vector<Node*> L;         // Search list

    // Step 1: Initialize L with nodes whose first coordinate matches any of the filters in F_q
    for (Node* s : S) {
        if (!s) continue;
        // Check if the first coordinate of the node matches any of the filters in the set F_q
        if (filters_q.find(s->coords[0]) != filters_q.end()) {
            L.push_back(s);
        }
    }

    // Priority queue for efficiently finding the closest unvisited node
    using NodeDistPair = pair<double, Node*>;
    priority_queue<NodeDistPair, vector<NodeDistPair>, greater<>> pq;

    // Prepopulate the priority queue with the initial nodes in L
    for (Node* node : L) {
        pq.emplace(euclidean(node, x_q), node);
    }

    while (!L.empty() && any_of(L.begin(), L.end(), [&](Node* p) { return V.find(p) == V.end(); })) {
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

        // Add out-neighbors of `p_star` that match the query's filter (i.e., their first coordinate is in F_q)
        for (Node* neighbor : p_star->out_neighbors) {
            if (V.find(neighbor) == V.end() && filters_q.find(neighbor->coords[0]) != filters_q.end()) {
                L.push_back(neighbor);
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

    return L; // Return the `k` closest points from `L`
}

