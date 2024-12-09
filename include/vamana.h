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
#include <unordered_map>
#include <numeric>
#include <vector>

using namespace std;

struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;  //coordinates
    vector<Node*> out_neighbors; 
    float filter;
};

struct DirectedGraph {
    unordered_map<Node*, unordered_set<Node*>> adjacency_list;

    size_t size() const {
        return adjacency_list.size();
    }
};

//helper functions
float euclidean(const Node* a, const Node* b);
vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors);
bool compare_distance(Node* node1, Node* node2);
Node* createNode(unsigned int id, float filter, const vector<float>& coords, const vector<Node*>& neighbors);
void fisherYatesShuffle(vector<Node*>& databasePoints);
void initializeRandomGraph(vector<Node*>& nodes, unsigned int R);

//medoid functions
Node* findCentroid(const vector<Node*>& cluster);
vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations = 100);
int approximateMedoid(const vector<Node*>& nodes, int k);
unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau);

//ex1
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) ;
void RobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);
void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n);

//ex2
void FilteredRobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);
vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& query_filter);
DirectedGraph FilteredVamana(vector<Node*>& databasePoints,int k, unsigned int L, unsigned int R, float alpha, unsigned int tau);
void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched);

//file manipulation functions
vector<vector<int>> loadIvecs(const string& filename);
vector<vector<float>> loadFvecs(const string& filename);
vector<Node*> load_bvecs(const string& filename);



unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau);
// Define a threshold for comparing floats (useful for floating-point equality checks)
constexpr float EPSILON = 1e-6;
vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes);

vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path);

vector<Node*> CreateGraph(const vector<vector<float>>& vectors);

vector<vector<float>> ReadGraph(const string &file_path);