#include <iostream>
#include <set>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

struct Node{
    int id;
    vector<double> coords;
    vector<Node*> neighbors;
};

double calculate_distance(const Node* a, const Node* b) {
    double distance = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        distance += (a->coords[i] - b->coords[i]) * (a->coords[i] - b->coords[i]);
    }
    return sqrt(distance);
}

void greedysearh(Node* s, Node* xq, int k, int l){
    unordered_set<Node*> V;                                        // Visited nodes
    vector<Node*> result;                                              // k-closest nodes
    vector<Node*> L = {s};   

    //  while search list is not empty and we have not found all the closest neighbors
    while (!L.empty() && result.size() < k) {

        //find the closest unvisited node to x_q
        Node* p_star = nullptr;
        double min_distance = numeric_limits<double>::max();

        //  traverse the search list L
        for (Node* p : L) {
            //  and calculate the distance from x_q for the nodes that are not visited
            if (V.find(p) == V.end()) {
                double distance = calculate_distance(p, xq);
                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }
        
        if(p_star){
            V.insert(p_star);
            for (Node* p : p_star->neighbors){
                /* code */
            }
            
        }

    }


}

int main(){
    vector<Node*> nodes;
    Node s, xq, n1, n2, n3, n4, n5;
    int k, list_size;
    greedysearh(&s, &xq, k, list_size);

}
