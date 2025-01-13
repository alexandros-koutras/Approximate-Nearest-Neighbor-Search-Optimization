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
#include <cstdlib>
#include <random>
#include <ctime>
#include <sstream>
#include <string>
#include <unordered_map>
#include <numeric>
#include <vector>
#include <getopt.h>
#include <chrono>
#include <thread>
#include <mutex> 


using namespace std;

// Define a threshold for comparing floats
constexpr float EPSILON = 1e-6;


struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors; 
    float filter;
};

struct DirectedGraph {
    unordered_map<Node*, unordered_set<Node*>> adjacency_list;

    size_t size() const {
        return adjacency_list.size();
    }
};


vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size);

float euclidean(const Node* a, const Node* b);

bool compare_distance(Node* node1, Node* node2);

void RobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n, int medoidCase, int subsetSize = 10);

vector<vector<float>> ReadBin(const string &file_path, const int num_dimensions);

void SaveVectorToBinary(const vector<vector<float>>& vectors, const string& file_path);

vector<vector<float>> ReadGroundTruth(const string& file_path);

vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors);

vector<Node*> createQueriesFromVectors(const vector<vector<float>>& vectors);

vector<vector<float>> createVectorFromNodes(const vector<Node*>& nodes);

vector<Node*> CreateGraph(vector<vector<float>> vectors);

vector<vector<float>> ReadGraph(const string &file_path);

vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& query_filter);

void FilteredRobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);

void StitchedVamana(vector<Node*>& nodes, float a, int L_small, int R_small, int R_stiched);

unordered_map<float,  unsigned int> findmedoid(const vector<Node*>& P, unsigned int tau);

DirectedGraph FilteredVamana(vector<Node*>& databasePoints,int k, unsigned int L, unsigned int R, float alpha, unsigned int tau);

vector<vector<float>> brute_force(vector<Node*>& nodes, vector<Node*>& queries);

Node* findCentroid(const vector<Node*>& cluster);

vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations = 100);

int approximateMedoid(const vector<Node*>& nodes, int k);

void fisherYatesShuffle(vector<Node*>& databasePoints);

void initializeRandomGraph(vector<Node*>& nodes, unsigned int R);