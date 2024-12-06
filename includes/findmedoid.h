#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <iostream>

using namespace std;

struct Node{
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors;
};

// Define a threshold for comparing floats (useful for floating-point equality checks)
constexpr float EPSILON = 1e-6;
unordered_map<float, Node*> findmedoid(const vector<Node*>& P, unsigned int tau, const unordered_set<float>& all_filters);