#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdlib>  // For rand() and srand()

using namespace std;

struct Node{
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors;
};


double euclidean(const Node* a, const Node* b);
vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& query_filter);
