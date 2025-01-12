#include "../include/vamana.h"


void StitchedVamana(std::vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    unordered_set<float> uniqueFilters;
    for (Node* n : nodes) {
        uniqueFilters.insert(n->filter);
    }

    // Organize nodes by their filter
    unordered_map<float, vector<Node*>> commonFilter;
    for (Node* n : nodes) {
        commonFilter[n->filter].push_back(n);
    }

    // Process filters in parallel
    vector<thread> threads;
    for (float filter : uniqueFilters) {
        threads.emplace_back([&, filter]() {
            VamanaIndexingAlgorithm(commonFilter[filter], 100, L_small, R_small, a, commonFilter[filter].size());
        });
    }

    // Wait for all threads to complete
    for (thread& t : threads) {
        t.join();
    }

    // Parallel pruning
    threads.clear();  // Reuse the threads vector for pruning
    for (Node* n : nodes) {
        threads.emplace_back([&, n]() {
            FilteredRobustPrune(n, n->out_neighbors, a, R_stiched);
        });
    }

    // Wait for all pruning threads to complete
    for (thread& t : threads) {
        t.join();
    }
}