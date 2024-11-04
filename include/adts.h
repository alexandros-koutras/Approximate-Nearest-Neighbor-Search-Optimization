#include <iostream>
#include <vector>

using namespace std;


struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors;
};

struct Graph {
    vector<Node> vertices;
    int mun_nodes;
    int a;
    int k;
    int R;
    int L;
};
