#include "vamana.h"

void RobustPrune(Node* node, vector<Node*>& possible_neighbours, double a, int max_neighbours);

vector<Node*> GreedySearch(Node* s, const Node* x_q, int k, int list_size);

void initializeRandomGraph(vector<Node*>& nodes, int R);

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int L, int R, double a, int n);