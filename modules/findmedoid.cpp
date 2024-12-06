#include "../includes/findmedoid.h"

// FindMedoid implementation
unordered_map<float, Node*> findmedoid(const vector<Node*>& P, unsigned int tau, const unordered_set<float>& all_filters) {
    unordered_map<float, Node*> M; // Map of filters to medoids
    unordered_map<Node*, unsigned int> T; // Counter for visits to each node

    // Initialize T with zeros for all points
    for (const auto& node : P) {
        T[node] = 0;
    }

    // Random engine for sampling
    random_device rd;
    mt19937 gen(rd());

    // Process each filter
    for (const auto& f : all_filters) {
        vector<Node*> P_f; // Points matching filter f

        // Collect all points matching the current filter
        for (const auto& node : P) {
            if (fabs(node->coords[0] - f) < EPSILON) { 
                P_f.push_back(node);
            }
        }

        if (P_f.empty()) {
            cerr << "Warning: No points found for filter: " << f << endl;
            continue; // Skip if no points match the filter
        }

        // Randomly sample τ points from P_f
        vector<Node*> R_f;
        if (P_f.size() <= tau) {
            R_f = P_f; // If P_f has fewer than τ points, use them all
        } else {
            // Randomly select τ points
            shuffle(P_f.begin(), P_f.end(), gen);
            R_f.assign(P_f.begin(), P_f.begin() + tau);
        }

        // Find the point with the minimum count in T
        auto p_star = *min_element(R_f.begin(), R_f.end(),
            [&](Node* a, Node* b) {
                return T[a] < T[b];
            });

        // Update M and T
        M[f] = p_star;
        T[p_star]++;
    }

    return M;
}
