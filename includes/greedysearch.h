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