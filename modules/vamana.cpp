#include "../include/vamana.h"

//random R-regulated directed graph
void initializeRandomGraph(vector<Node*>& nodes, unsigned int R) {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (R >= nodes.size()) {
        //cerr << "Error: R must be less than the number of nodes." << endl;
        return;
    }

    for (Node* node : nodes) {
        unordered_set<int> neighbors;
        
        while (neighbors.size() < R) {
            unsigned int random_index = rand() % nodes.size();
            if (random_index != node->id && neighbors.find(random_index) == neighbors.end()) {
                node->out_neighbors.push_back(nodes[random_index]);
                neighbors.insert(random_index);
            }
        }
    }
}

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n, int medoidCase, int subsetSize) {
    //Step 1: Initialize a random R directed graph
    initializeRandomGraph(nodes, R);

    //Step 2: Find the medoid s of the dataset 
    Node* s = nullptr;

    if (n == 0)
        return;     //empty

    if (medoidCase== 0) {
        // Case 0: Select a random point
        s = nodes[rand() % n];
    } else if (medoidCase == 1) {
        // Case 1: Select a random subset and compute medoid
        vector<Node*> subset;
        for (int i = 0; i < subsetSize && i < n; i++) {
            subset.push_back(nodes[rand() % n]);
        }

        // Compute medoid for the subset
        double min_dist = numeric_limits<double>::max();
        int medoidIndex = 0;
        for (int i = 0; i < (int)subset.size(); i++) {
            double total_dist = 0;
            for (int j = 0; j < (int)subset.size(); j++) {
                if (i != j) {
                    total_dist += euclidean(subset[i], subset[j]);
                }
            }
            if (total_dist < min_dist) {
                min_dist = total_dist;
                medoidIndex = i;
            }
        }
        s = subset[medoidIndex];
    } else {
        // Case 2: Use the original full dataset medoid calculation
        int medoidIndex = 0;
        double min_dist = numeric_limits<double>::max();
        for (int i = 0; i < n; i++) {
            double total_dist = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    total_dist += euclidean(nodes[i], nodes[j]);
                }
            }
            if (total_dist < min_dist) {
                min_dist = total_dist;
                medoidIndex = i;
            }
        }
        s = nodes[medoidIndex];
    }

    // cout << "The medoid point found: " << endl;
    // cout << "[ ";

    // for (unsigned int i = 0; i < s->coords.size(); i++) {
    //     if (i == s->coords.size() - 1) {
    //         cout << s->coords.at(i);
    //         break;
    //     }
    //     cout << s->coords.at(i) << ", ";
    // }
    // cout << "]" << endl;

    //Step 3: Iterate through the dataset in a random order
    vector<int> permutation(n);         //list of all indices

    for(int i = 0; i < n; i++){     //populate the list with indices from 0 to n-1
        permutation[i]=i;
    }

    //fisher-yates shuffles //random_graph()
    for(int i = n - 1; i > 0; i--){
        int j = rand() % (i + 1);//random num from 0 to i
        swap(permutation[i], permutation[j]);
    }
    
    for (int i : permutation) {
        Node* p = nodes[i];
        
        //Run GreedySearch to find the visited set V_p
        vector<Node*> V_p = GreedySearch(s, p, 1, L);

        //Run RobustPrune on p with V_p, a, and R
        FilteredRobustPrune(p, V_p, a, R);

        //Add reverse edges 
        for (Node* neighbor : p->out_neighbors) {

            // If neighbor exceeds max degree R, apply RobustPrune
            if (neighbor->out_neighbors.size() + 1 > static_cast<size_t>(R)) {
            //Call RobustPrune with the current neighbors plus the new neighbor candidate p
                neighbor->out_neighbors.push_back(p);  //Temporarily add p
                FilteredRobustPrune(neighbor, neighbor->out_neighbors, a, R);
            } else {
            //Safe to add p directly without exceeding R
                neighbor->out_neighbors.push_back(p);
            }

        }
    }
}