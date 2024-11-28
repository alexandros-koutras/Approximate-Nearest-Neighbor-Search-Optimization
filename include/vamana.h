#pragma once

#include <vector>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <cstdlib>  // For rand() and srand()
#include <random>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

struct Node {
    unsigned int id;    
    float distance;                                         
    vector<double> coords;  //coordinates
    vector<Node*> out_neighbors; 

    ~Node() {
        // Clear neighbors if needed (e.g., no shared ownership)
        out_neighbors.clear();
    }
};

double euclidean(const Node* a, const Node* b);

vector<Node*> load_ivecs(const string& filename);

vector<Node*> load_fvecs(const string& filename);

vector<Node*> load_bvecs(const string& filename);

void RobustPrune(Node* node, vector<Node*> possible_neighbours, double a, unsigned int max_neighbours);

bool compare_distance(Node* node1, Node* node2);

vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size);

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, double a,int n);

void initializeRandomGraph(vector<Node*>& nodes, unsigned int R) ;

Node* create_node(unsigned int id, const vector<double>& coords);

double computeRecall(const vector<int>& groundTruth, const vector<Node*>& retrievedNeighbors) {
    int truePositiveCount = 0;
    unordered_set<int> retrievedIds;

    // Store retrieved neighbor IDs in a set for fast lookup
    for (Node* node : retrievedNeighbors) {
        retrievedIds.insert(node->id);
    }

    // Count the number of true positives (common neighbors)
    for (int gtId : groundTruth) {
        if (retrievedIds.find(gtId) != retrievedIds.end()) {
            truePositiveCount++;
        }
    }

    // Recall is the fraction of true positive neighbors
    return static_cast<double>(truePositiveCount) / retrievedNeighbors.size();
}




