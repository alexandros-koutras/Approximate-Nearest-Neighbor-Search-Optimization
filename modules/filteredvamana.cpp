#include "../include/vamana.h" 

// Fisher-Yates Shuffle
void fisherYatesShuffle(vector<Node*>& databasePoints) {
    int n = databasePoints.size();
    vector<int> permutation(n);

    // Populate with indices 0 to n-1
    for (int i = 0; i < n; i++) {
        permutation[i] = i;
    }

    // Perform Fisher-Yates shuffle
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Random number between 0 and i
        swap(permutation[i], permutation[j]);
    }

    // Rearrange the database points based on shuffled indices
    vector<Node*> shuffledDatabase(n);
    for (int i = 0; i < n; i++) {
        shuffledDatabase[i] = databasePoints[permutation[i]];
    }

    // Copy the shuffled points back into the original databasePoints vector
    databasePoints = shuffledDatabase;
}

//Filtered Vamana
void FilteredVamana(vector<Node*>& databasePoints,int k, unsigned int L, unsigned int R, float alpha, unsigned int tau) {    
    //find medoids for every filter
    unordered_map<float, unsigned int> medoids = findmedoid(databasePoints, tau);

    // Randomize database points
    fisherYatesShuffle(databasePoints);

    // Iterate over the points
    for (Node* point : databasePoints) {
        //Define S_{F_x} as the start nodes for filtering
        vector<Node*> S_Fx; //Using the medoid as st(f)

        // Add medoids corresponding to the filter of the point
        float filter = point->filter;
        if (medoids.find(filter) != medoids.end()) {
            unsigned int medoid_id = medoids[filter];
            auto it = find_if(databasePoints.begin(), databasePoints.end(),[&](Node* n) { return n->id == medoid_id; });
            if (it != databasePoints.end()) {
                S_Fx.push_back(*it);  
            }
        }

        //FilteredGreedySearch
        unordered_set<float> query_filter = {point->filter};
        vector<Node*> V_Fx = FilteredGreedySearch(S_Fx, point, 0, L, query_filter);
        
        // Add the out-neighbors to the node's `out_neighbors`
        for (Node* neighbor : V_Fx) {
            point->out_neighbors.push_back(neighbor);
        }

        //FilteredRobustPrune
        FilteredRobustPrune(point, V_Fx, alpha, R);
    }
}