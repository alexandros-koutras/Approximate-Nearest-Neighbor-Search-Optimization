#include <iostream>
#include <unordered_set>
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
    vector<Node*> neighbors;
};

// Function to read .ivecs file
vector<Node*> load_ivecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            int value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(int))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << value << endl;
        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}

// Function to read .fvecs file
vector<Node*> load_fvecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        //  Read dimensions
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;  
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            //  Read each coordinate
            float value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(float))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            //cout << "Value " << i << ": " << value << endl;

        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}

// Function to read .bvecs file
vector<Node*> load_bvecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            unsigned char value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(unsigned char))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << static_cast<int>(value) << endl;
        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}


void assign_random_neighbors(vector<Node*>& nodes, int num_neighbors) {
    srand(time(nullptr)); // Seed for randomness

    for (Node* node : nodes) {
        vector<Node*> neighbors;

        while (neighbors.size() < num_neighbors) {
            Node* potential_neighbor = nodes[rand() % nodes.size()];
            
            // Avoid self-loops and duplicate neighbors
            if (potential_neighbor != node && 
                find(neighbors.begin(), neighbors.end(), potential_neighbor) == neighbors.end()) {
                neighbors.push_back(potential_neighbor);
            }
        }
        
        node->neighbors = neighbors;  // Assign the selected neighbors to the node
    }
}

// Distance between two nodes (using Euclidean distance)
double calculate_distance(const Node* a, const Node* b) {
    double distance = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        distance += (a->coords[i] - b->coords[i]) * (a->coords[i] - b->coords[i]);
    }
    return sqrt(distance);
}

// GreedySearch αλγόριθμος
vector<Node*> GreedySearch(Node* s, const Node* x_q, int k, int list_size) {
    if (!s) {
        cerr << "Error: Starting node is null." << endl;
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;                                        // Visited nodes
    vector<Node*> result;                                              // k-closest nodes
    vector<Node*> L = {s};                                             // Η λίστα αναζήτησης ξεκινά με τον κόμβο s

    //  while search list is not empty and we have not found all the closest neighbors
    while (!L.empty() && result.size() < k) {

        //find the closest unvisited node to x_q
        Node* p_star = nullptr;
        double min_distance = numeric_limits<double>::max();

        //  traverse the search list L
        for (Node* p : L) {
            //  and calculate the distance from x_q for the nodes that are not visited
            if (V.find(p) == V.end()) {
                double distance = calculate_distance(p, x_q);
                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }

        if (p_star) {
            //  If we have found p_star, we visit it and add it to the result list
            V.insert(p_star); 
            result.push_back(p_star); 

            //  update the search list with p_star's out-neighbors
            for (Node* neighbor : p_star->neighbors) {
                //  if they are not visited add them to the search list L
                if (V.find(neighbor) == V.end()) {
                    L.push_back(neighbor);
                }
            }

            // if search list overcome l, we maintain the l closest nodes
            if (L.size() > list_size) {

                //using the function nth_element to sort only the list_size part of the vector 
                nth_element(L.begin(), L.begin() + list_size, L.end(), [&](Node* a, Node* b) {
                        return calculate_distance(a, x_q) < calculate_distance(b, x_q);
                    });
                L.resize(list_size); // keep the list_size
            }

        } else {
            // if p_star not found we stop the searching
            break; 
        }
    }

    return result; 
}

#ifndef TESTING_MODE
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <base.fvecs> <query.fvecs> <k> <l>" << endl;
        return 1;
    }

    // Load base vectors
    string base_file = argv[1];
    vector<Node*> nodes = load_fvecs(base_file);

    // Load query vectors
    string query_file = argv[2];
    vector<Node*> query_nodes = load_fvecs(query_file);

    // Parameters
    int k = stoi(argv[3]);  // number of closest nodes to find
    int l = stoi(argv[4]);  // max size of search list

    // Assign random neighbors for each node with a fixed number, e.g., 5
    assign_random_neighbors(nodes, 5);

    // Testing neighbor assignment
    cout << "Neighbor assignments:" << endl;
    for (Node* node : nodes) {
        cout << "Node " << node->id << " neighbors: ";
        for (Node* neighbor : node->neighbors) {
            cout << neighbor->id << " ";
        }
        cout << endl;
    }

    // Perform GreedySearch for each query vector
    for (Node* query : query_nodes) {
        vector<Node*> nearest_neighbors = GreedySearch(nodes[0], query, k, l);  // Starting from node 0

        // Output the results (IDs of the nearest neighbors)
        cout << "Query " << query->id << ": ";
        for (Node* neighbor : nearest_neighbors) {
            cout << neighbor->id << " ";
        }
        cout << endl;
    }

    // Cleanup: free memory
    for (Node* node : nodes) delete node;
    for (Node* query : query_nodes) delete query;

    return 0;
}
#endif