#include "../include/vamana.h"

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    unordered_set<float> uniqueFilters;
    
    for (Node* n : nodes) {
        uniqueFilters.insert(n->filter);
    }

    // Organise them
    unordered_map<float, vector<Node*>> commonFilter;

    for (Node* n : nodes) {
        commonFilter[n->filter].push_back(n);
    }

    for (float filter : uniqueFilters) {
        VamanaIndexingAlgorithm(commonFilter[filter], 100, L_small, R_small, a, commonFilter[filter].size());
    }

    for (Node* n : nodes) {
        FilteredRobustPrune(n, n->out_neighbors, a, R_stiched);
    }
}