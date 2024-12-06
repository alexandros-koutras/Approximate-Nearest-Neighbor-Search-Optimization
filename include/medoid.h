#include "vamana.h"

Node* findCentroid(const vector<Node*>& cluster);

vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations);

int approximateMedoid(const vector<Node*>& nodes, int k);