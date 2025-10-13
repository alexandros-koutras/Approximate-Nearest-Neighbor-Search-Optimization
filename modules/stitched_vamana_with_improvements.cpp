#include "../include/vamana.h"

void StitchedVamana_WithImprovement(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stitched) {
    // Find all the unique filters
    unordered_set<float> uniqueFilters;
    unordered_map<float, vector<Node*>> commonFilter;

    for (Node* n : nodes) {
        uniqueFilters.insert(n->filter);
        commonFilter[n->filter].push_back(n);
    }

    cout << "Unique filters: " << uniqueFilters.size() << endl;

     // Randomly interconnect filters by adding random edges between filters
    vector<float> filters(uniqueFilters.begin(), uniqueFilters.end());

     // Shuffle filters
    for (size_t i = 0; i < filters.size(); ++i) {
        size_t j = rand() % filters.size();
        swap(filters[i], filters[j]);
    }

    for (size_t i = 0; i < filters.size(); ++i) {
        for (size_t j = i + 1; j < filters.size(); ++j) {
            vector<Node*>& filterGroup1 = commonFilter[filters[i]];
            vector<Node*>& filterGroup2 = commonFilter[filters[j]];

            // Randomly connect a few nodes between filter groups
            for (int k = 0; k < R_stitched; ++k) {
                if (filterGroup1.empty() || filterGroup2.empty()) break;

                Node* node1 = filterGroup1[rand() % filterGroup1.size()];
                Node* node2 = filterGroup2[rand() % filterGroup2.size()];

                node1->out_neighbors.push_back(node2);
                node2->out_neighbors.push_back(node1);
            }
        }
    }

    cout << "Added random edges between filters\n";

    for (float filter : uniqueFilters) {
        VamanaIndexingAlgorithm(commonFilter[filter], 20, L_small, R_small, a, commonFilter[filter].size(), 1, 3500);
    }

    // cout << "All Good Vamana\n";

    for (Node* n : nodes) {
        FilteredRobustPrune(n, n->out_neighbors, a, R_stitched);
        // Filter out neighbors with different filters after pruning
        n->out_neighbors.erase(
            std::remove_if(n->out_neighbors.begin(), n->out_neighbors.end(),
                        [n](Node* neighbor) { return neighbor->filter != n->filter; }),
            n->out_neighbors.end());
    }

    // cout << "finish\n";
}