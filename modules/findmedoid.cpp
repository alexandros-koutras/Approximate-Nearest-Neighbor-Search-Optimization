#include "../include/vamana.h"

// FindMedoid implementation
unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau) {
    unordered_map<float,  unsigned int> M; // Map of filters to medoids
    unordered_map< unsigned int, unsigned int> T; // Counter for visits to each node
    unordered_set<float> all_filters;

    // Initialize T with zeros for all points
    for (const auto& node : P) {
        T[node->id] = 0;
        all_filters.insert(node->filter); 
    }

    // Random engine for sampling
    random_device rd;
    mt19937 gen(rd());



    // Process each filter
    for (const auto& f : all_filters) {
        vector<unsigned int> P_f; // Points matching filter f

        // Collect all points matching the current filter
        for (const auto& node : P) {
            if (fabs(node->filter - f) < EPSILON) { 
                P_f.push_back(node->id);
            }
        }

        if (P_f.empty()) {
            cerr << "Warning: No points found for filter: " << f << endl;
            continue; // Skip if no points match the filter
        }

        // Randomly sample τ points from P_f
        vector<unsigned int> R_f;
        if (P_f.size() <= tau) {
            R_f = P_f; // If P_f has fewer than τ points, use them all
        } else {
            // Randomly select τ points
            shuffle(P_f.begin(), P_f.end(), gen);
            R_f.assign(P_f.begin(), P_f.begin() + tau);
        }

        // Find the point with the minimum count in T
        auto p_star = *min_element(
            R_f.begin(), R_f.end(),
            [&](unsigned int a, unsigned int b) {
                return T[a] < T[b];
            });

        // Update M and T
        M[f] = p_star;
        T[p_star]++;
    }

    return M;
}