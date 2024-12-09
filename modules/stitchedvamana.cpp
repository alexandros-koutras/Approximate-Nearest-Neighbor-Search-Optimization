#include "../include/vamana.h"

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    // Organise them
    unordered_map<float, vector<Node*>> commonFilter;

    for (Node* n : nodes) {
        commonFilter[n->filter].push_back(n);
    }

    for (auto& [label, subset] : commonFilter) {
        // Build a graph for nodes with the same label
        VamanaIndexingAlgorithm(subset, 2, L_small, R_small, a, subset.size());
    }

    for (Node* n : nodes) {
        FilteredRobustPrune(n, n->out_neighbors, a, R_stiched);
    }
}
