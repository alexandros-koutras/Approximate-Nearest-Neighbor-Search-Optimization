#include "../include/vamana.h"

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    // Organise them
    unordered_map<float, vector<Node*>> commonFilter;


    for (Node* n : nodes) {
        if (n->filter == 123) {
            cout << "Node " << n->id << " has filter: " << n->filter << endl;
        }
        commonFilter[n->filter].push_back(n);
    }


    for (auto& [label, subset] : commonFilter) {
        cout << "Subset with filter " << label << " has " << subset.size() << " nodes." << endl;
        if (subset.size() <  150) {
            continue;
        }
        VamanaIndexingAlgorithm(subset, 10, L_small, R_small, a, subset.size());
    }

    for (Node* n : nodes) {
        FilteredRobustPrune(n, n->out_neighbors, a, R_stiched);
    }
}
