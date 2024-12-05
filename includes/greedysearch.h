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
vector<Node*> load_fvecs(const string& filename);
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size);
