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

using namespace std;

struct Node {//or class
    unsigned int id;                                             //node's id
    vector<double> coords;               //coordinates
    vector<Node*> out_neighbors; 
    Node(int id):id(id){}
    
    void add_neighbour(Node* neighbor) {
        if(find(out_neighbors.begin(),out_neighbors.end(),neighbor)==out_neighbors.end()){
            out_neighbors.push_back(neighbor);
        }
    }

    // void remove_neighbor(int neighbor){
    //     out_neighbors.erase(remove(out_neighbors.begin(),out_neighbors.end(),neighbor),out_neighbors.end());
    // }

    vector<double> get_coordinates() const {
        return coords;
    }          //out-neighbors
};

double euclidean(const Node* a, const Node* b);

vector<vector<int>> ivecs_read(const string& filename, int a = 1, int b = -1);

vector<Node*> load_vecs(const string& filename);

// Distance between two nodes (using Euclidean distance)
double euclidean(const Node* a, const Node* b) {
    double sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        sum +=pow (a->coords[i] - b->coords[i],2);
    }
    return sqrt(sum);
}

