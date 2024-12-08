#pragma once

#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
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
#include <vector>

using namespace std;

struct Node{
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors;
    unordered_set<string> tags;
};

struct DirectedGraph {
    unordered_map<Node*, unordered_set<Node*>> adjacency_list;

    size_t size() const {
        return adjacency_list.size();
    }
};

//helper functions
double euclidean(const Node* a, const Node* b);
vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors);
bool compare_distance(Node* node1, Node* node2);
Node* createNode(unsigned int id, const vector<float>& coords, const unordered_set<string>& tags = {});
void fisherYatesShuffle(vector<Node*>& databasePoints);
void initializeRandomGraph(vector<Node*>& nodes, unsigned int R);

//medoid functions
Node* findCentroid(const vector<Node*>& cluster);
vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations = 100);
int approximateMedoid(const vector<Node*>& nodes, int k);
Node* findMedoid(const vector<Node*>& points,int k);

//ex1
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) ;
void RobustPrune(Node* node, vector<Node*> possible_neighbours, double a, unsigned int max_neighbours);
void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n);

//ex2
void FilteredRobustPrune(Node* node, vector<Node*> possible_neighbours, float a, unsigned int max_neighbours);
vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<string>& query_filter);
DirectedGraph FilteredVamana(vector<Node*>& databasePoints,int k, unsigned int L, unsigned int R, float alpha);
DirectedGraph StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched);

//file manipulation functions
vector<vector<int>> loadIvecs(const string& filename);
vector<vector<float>> loadFvecs(const string& filename);
vector<Node*> load_bvecs(const string& filename);

