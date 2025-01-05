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
#include <cstdlib>
#include <random>
#include <ctime>
#include <sstream>
#include <string>
#include <unordered_map>
#include <numeric>

using namespace std;

// Define a threshold for comparing floats
constexpr float EPSILON = 1e-6;


struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors; 
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


unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau);

vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes);

vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path);

vector<Node*> CreateGraph(vector<vector<float>> vectors);

vector<vector<float>> ReadGraph(const string &file_path);

vector<vector<uint32_t>> bruteForceKNNWithFilter(const vector<Node*>& dataset, const vector< Node*>& queries, int k) ;

vector<Node*> createQueriesFromVectors(const vector<vector<float>>& vectors) ;

void SaveKNN(const vector<vector<uint32_t>> &knns,const string &path = "output.bin");

vector<vector<float>> ReadGroundTruth(const string& file_path);