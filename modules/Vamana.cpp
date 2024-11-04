#include "../include/vamana.h"
#include "../include/medoid.h"

//random R-regulated directed graph
void initializeRandomGraph(vector<Node*>& nodes, unsigned int R) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (Node* node : nodes) {
        unordered_set<int> neighbors;
        
        while (neighbors.size() < R) {
            unsigned int random_index = rand() % nodes.size();
            if (random_index != node->id && neighbors.find(random_index) == neighbors.end()) {
                Node* neighbor = nodes[random_index];
                node->out_neighbors.push_back(neighbor);
                neighbors.insert(random_index);
            }
        }
    }
}

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, double a,int n) {
    //Step 1: Initialize a random R directed graph
    initializeRandomGraph(nodes, R);

    //Step 2: Find the medoid s of the dataset 
    if (n == 0)return;//empty
        //old medoid time consuming
        /*int medoid = 0;
        double min_dist = numeric_limits<float>::max();
        for (int i = 0; i < n; i++){
            double total_dist  =0;
            for(int j = 0; j < n; j++){
                if(i != j){
                    total_dist += euclidean(nodes[i],nodes[j]);
                }
            }
            if (total_dist < min_dist){
                min_dist = total_dist;
                medoid = i;
            }
        }*/
    int medoid=approximateMedoid(nodes,k);
    Node* s=nodes[medoid];

    cout << "The medoid point found: " << endl;
    cout << "[ ";
    for (unsigned int i = 0; i < s->coords.size(); i++) {
        if (i == s->coords.size() - 1) {
            cout << s->coords.at(i);
        }
        cout << s->coords.at(i) << ", ";
    }
    cout << "]" << endl;

    //Step 3: Iterate through the dataset in a random order
     vector<int> permutation(n);//list of all indices
        for(int i = 0; i < n; i++){//populate the list with indices from 0 to n-1
            permutation[i]=i;
        }
        //fisher-yates shuffles //random_graph()
        for(int i = n-1;i>0;i--){
            int j = rand()%(i+1);//random num from 0 to i
            swap(permutation[i],permutation[j]);
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
