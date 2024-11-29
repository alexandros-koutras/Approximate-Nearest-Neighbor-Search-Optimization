#include "../include/vamana.h" // Include FilteredGreedySearch and other dependencies

// Function to find the medoid
Node* findMedoid(const vector<Node*>& points,int k) {
    int medoidIndex = approximateMedoid(points, k); // Use clustering to determine approximate medoid
    return points[medoidIndex];
}

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

DirectedGraph FilteredVamana(vector<Node*>& databasePoints,int k, unsigned int L, unsigned int R, float alpha) {
    // Step 1: Initialize Graph
    DirectedGraph G;

    Node* medoid = findMedoid(databasePoints,k);

    // Randomize database points using Fisher-Yates shuffle
    fisherYatesShuffle(databasePoints);

    // Create initial structures
    for (Node* point : databasePoints) {
        G.adjacency_list[point]= unordered_set<Node*>();
    }

    // Iterate over the points in the shuffled order
    for (Node* point : databasePoints) {
        // Step 2: Define S_{F_x} as the start nodes for filtering
        vector<Node*> S_Fx = {medoid}; // Assume medoid acts as st(f) for simplicity (expand as necessary)

        // Step 3: Perform FilteredGreedySearch
        vector<Node*> V_Fx = FilteredGreedySearch(S_Fx, point, 0, L, point->tags);

        // Step 4: Update neighbors of the current point using FilteredRobustPrune
        FilteredRobustPrune(point, V_Fx, alpha, R);

        // Add these neighbors to the graph
        for (Node* neighbor : V_Fx) {
            G.adjacency_list[point].insert(neighbor);
        }

        // Step 5: Update neighbors for each out-neighbor
        for (Node* neighbor : G.adjacency_list[point]) {
            G.adjacency_list[neighbor].insert(point);

            // Check if the out-degree exceeds R
            if (G.adjacency_list[neighbor].size() > R) {
                vector<Node*> neighborList(G.adjacency_list[neighbor].begin(), G.adjacency_list[neighbor].end());
                FilteredRobustPrune(neighbor, neighborList, alpha, R);
                G.adjacency_list[neighbor] = unordered_set<Node*>(neighborList.begin(), neighborList.end());
            }
        }
    }

    return G;
}