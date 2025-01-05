#include "../include/vamana.h"

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    unordered_set<float> uniqueFilters;
    
    for (Node* n : nodes) {
        uniqueFilters.insert(n->filter);
    }

    cout << "Unique filters: " << uniqueFilters.size() << endl;

    // Organise them
    unordered_map<float, vector<Node*>> commonFilter;

    for (Node* n : nodes) {
        commonFilter[n->filter].push_back(n);
    }

    cout << "All Good\n";

    for (float filter : uniqueFilters) {
        VamanaIndexingAlgorithm(commonFilter[filter], 20, L_small, R_small, a, commonFilter[filter].size());
    }

    cout << "All Good Vamana\n";

    for (Node* n : nodes) {
        RobustPrune(n, n->out_neighbors, a, R_stiched);
    }

    cout << "finish\n";
}
