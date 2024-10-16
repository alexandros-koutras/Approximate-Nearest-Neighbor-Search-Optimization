#include <iostream>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

struct Node {
    int id;                                             //node's id
    vector<double> coords;               //coordinates
    vector<Node*> neighbors;           //out-neighbors
};

vector<vector<int>> ivecs_read(const string& filename, int a = 1, int b = -1) {
    ifstream file(filename, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return {};
    }

    // Read the dimension of the vectors
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));

    // Calculate the size of each vector in bytes
    int vecsizeof = 1 * sizeof(int) + d * sizeof(int); // 4 bytes for dimension + d * 4 bytes for vector components

    // Move to the end of the file to find how many vectors are in the file
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int bmax = fileSize / vecsizeof;

    if (b == -1) {
        b = bmax;
    }

    if (a < 1 || b > bmax || b < a) {
        cerr << "Error: Invalid bounds" << endl;
        return {};
    }

    // Go to the position of the first vector to read
    file.seekg((a - 1) * vecsizeof, ios::beg);

    // Prepare the output vector
    vector<vector<int>> vectors(b - a + 1, vector<int>(d));

    for (int i = 0; i < b - a + 1; ++i) {
        // Read the dimension (should be the same for all vectors)
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        assert(dim == d); // Ensure all vectors have the same dimension

        // Read the vector components
        file.read(reinterpret_cast<char*>(vectors[i].data()), d * sizeof(int));
    }

    file.close();
    return vectors;
}


vector<vector<float>> read_fvecs(const string &filename, size_t num_vectors = 0) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Unable to open file " + filename);
    }

    int d;
    // Διαβάζουμε τη διάσταση του πρώτου διανύσματος
    file.read(reinterpret_cast<char*>(&d), sizeof(int));
    
    // Υπολογίζουμε το μέγεθος ενός διανύσματος
    size_t vec_size = d * sizeof(float);

    // Πάμε στο τέλος για να δούμε πόσα διανύσματα υπάρχουν αν δεν έχει δοθεί το πλήθος
    if (num_vectors == 0) {
        file.seekg(0, ios::end);
        size_t file_size = file.tellg();
        num_vectors = file_size / (sizeof(int) + vec_size);
        file.seekg(0, ios::beg);
    }

    vector<vector<float>> vectors(num_vectors, vector<float>(d));

    // Για κάθε διάνυσμα
    for (size_t i = 0; i < num_vectors; ++i) {
        file.read(reinterpret_cast<char*>(&d), sizeof(int));  // Ξαναδιαβάζουμε τη διάσταση
        file.read(reinterpret_cast<char*>(vectors[i].data()), vec_size);  // Διαβάζουμε το διάνυσμα
    }

    file.close();
    return vectors;
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
vector<Node*> GreedySearch(Node* s, const Node* x_q, int k, int l) {

    unordered_set<Node*> V;                                        // Visited nodes
    vector<Node*> result;                                              // k-closest nodes
    vector<Node*> L = {s};                                             // Η λίστα αναζήτησης ξεκινά με τον κόμβο s

    //uodate search list L function
    auto update_L = [&](Node* p) {

        //  travers the out-neighbors of the node
        for (Node* neighbor : p->neighbors) {
            //  if they are not visited add them to the search list L
            if (V.find(neighbor) == V.end()) {
                L.push_back(neighbor);
            }
        }

    };

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
            update_L(p_star); 

            // if search list overcome l, we maintain the l closest nodes
            if (L.size() > l) {
                nth_element(L.begin(), L.begin() + l, L.end(),
                    [&](Node* a, Node* b) {
                        return calculate_distance(a, x_q) < calculate_distance(b, x_q);
                    });
                L.resize(l); // Κρατάμε μόνο τα πρώτα l στοιχεία
            }
        } else {
            // if p_star not found we stop the searching
            break; 
        }
    }

    return result; 
}



int main() {
    try {
        // Read the fvecs dataset
        auto fvectors = read_fvecs("siftsmall/siftsmall_base.fvecs");
        cout << "Read " << fvectors.size() << " vectors from the fvecs file.\n";

        // Create a list of nodes from the vectors
        vector<Node> nodes(fvectors.size());
        for (size_t i = 0; i < fvectors.size(); ++i) {
            nodes[i].id = i;
            nodes[i].coords.resize(fvectors[i].size());
            copy(fvectors[i].begin(), fvectors[i].end(), nodes[i].coords.begin());
        }

        // For testing, let's connect each node to its next node as a neighbor
        for (size_t i = 0; i < nodes.size() - 1; ++i) {
            nodes[i].neighbors.push_back(&nodes[i + 1]);
        }

        // Use the first node as the start and the last node as the query node
        Node* start_node = &nodes[0];
        Node* query_node = &nodes.back();

        // Run the GreedySearch algorithm
        int k = 5; // Number of closest neighbors to find
        int l = 10; // Limit on the search list size
        vector<Node*> closest_neighbors = GreedySearch(start_node, query_node, k, l);

        // Print out the result
        cout << "Found " << closest_neighbors.size() << " closest neighbors:\n";
        for (const auto* neighbor : closest_neighbors) {
            cout << "Node ID: " << neighbor->id << ", Coordinates: ";
            for (double coord : neighbor->coords) {
                cout << coord << " ";
            }
            cout << endl;
        }

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
