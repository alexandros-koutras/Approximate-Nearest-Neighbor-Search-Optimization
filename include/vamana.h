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