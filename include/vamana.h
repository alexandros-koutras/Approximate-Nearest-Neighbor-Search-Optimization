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
#include <unordered_map>
#include <numeric>

using namespace std;

struct Node {
    unsigned int id;    
    float distance;                                         
    vector<float> coords;  //coordinates
    vector<Node*> out_neighbors; 
    float filter;
};

float euclidean(const Node* a, const Node* b);

vector<vector<int>> loadIvecs(const string& filename);

vector<vector<float>> loadFvecs(const string& filename);

vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors);

vector<Node*> load_bvecs(const string& filename);

void RobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);

bool compare_distance(Node* node1, Node* node2);

vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size);

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a,int n);

void initializeRandomGraph(vector<Node*>& nodes, unsigned int R) ;

Node* create_node(unsigned int id, const vector<float>& coords);

Node* findCentroid(const vector<Node*>& cluster);

vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations = 100);

int approximateMedoid(const vector<Node*>& nodes, int k);

vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched);

vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& query_filter) ;

// Define a threshold for comparing floats (useful for floating-point equality checks)
constexpr float EPSILON = 1e-6;

unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau, const unordered_set<float>& all_filters);