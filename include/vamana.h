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


vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size);

float euclidean(const Node* a, const Node* b);

bool compare_distance(Node* node1, Node* node2);

void RobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours);

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int k, int L, int R, float a, int n);

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