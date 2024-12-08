#include "../include/vamana.h"

//random R-regulated directed graph
void initializeRandomGraph(vector<Node*>& nodes, unsigned int R) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
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

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n) {
    //Step 1: Initialize a random R directed graph
    initializeRandomGraph(nodes, R);

    //Step 2: Find the medoid s of the dataset 
    if (n == 0)
        return;     //empty
        
    int medoid = approximateMedoid(nodes,k);
    Node* s = nodes[medoid];

    cout << "The medoid point found: " << endl;
    cout << "[ ";

    for (unsigned int i = 0; i < s->coords.size(); i++) {
        if (i == s->coords.size() - 1) {
            cout << s->coords.at(i);
            break;
        }
        cout << s->coords.at(i) << ", ";
    }
    cout << "]" << endl;

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
        RobustPrune(p, V_p, a, R);

        //Add reverse edges 
        for (Node* neighbor : p->out_neighbors) {

            // If neighbor exceeds max degree R, apply RobustPrune
            if (neighbor->out_neighbors.size() + 1 > static_cast<size_t>(R)) {
            //Call RobustPrune with the current neighbors plus the new neighbor candidate p
                neighbor->out_neighbors.push_back(p);  //Temporarily add p
                RobustPrune(neighbor, neighbor->out_neighbors, a, R);
            } else {
            //Safe to add p directly without exceeding R
                neighbor->out_neighbors.push_back(p);
            }

        }
    }
}

DirectedGraph StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched) {
    // Find all the unique filters
    DirectedGraph G;
    unordered_set<string> uniqueLabels;

    for (Node* n : nodes) {
        for (const string& tag : n->tags) {
            uniqueLabels.insert(tag);
        }
    }

    // Apply the Vamana indexing algorithm on each subset
    for (const string& label : uniqueLabels) {
        vector<Node*> subset;
        for (Node* n : nodes) {
            if (n->tags.count(label)) {
                subset.push_back(n);
            }
        }
        // Run the VamanaIndexingAlgorithm on this subset
        VamanaIndexingAlgorithm(subset, 100, L_small, R_small, a, 100);
        // After VamanaIndexingAlgorithm processes the subset, map the nodes' out_neighbors to the graph
        for (Node* n : subset) {
            G.adjacency_list[n] = unordered_set<Node*>(n->out_neighbors.begin(), n->out_neighbors.end());
        }
    }

    // Prune out neighbors to ensure out-degree constraints are respected
    for (Node* n : nodes) {
        FilteredRobustPrune(n, n->out_neighbors, a, R_stiched);

        // After pruning, add edges to the directed graph structure
        G.adjacency_list[n] = unordered_set<Node*>(n->out_neighbors.begin(), n->out_neighbors.end());
    }

    return G; // Return the constructed DirectedGraph
}