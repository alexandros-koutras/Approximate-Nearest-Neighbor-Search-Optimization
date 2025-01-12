#include "../include/acutest.h"

#include "../include/vamana.h"


vector<Node*> createNodesFromVectors(const vector<vector<float>>& vectors) {
    vector<Node*> nodes;

    for (size_t i = 0; i < vectors.size(); ++i) {
        Node* newNode = new Node;
        newNode->id = i;  // Use index as the ID
        newNode->filter = vectors[i].at(0);
        newNode->coords.assign(vectors[i].begin() + 2, vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}

float euclidean(const Node* a, const Node* b) {
    float sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        float diff = a->coords[i] - b->coords[i];
        sum += diff * diff;
    }
    return sum;
}


// Compare function for 2 nodes based to their distance to another common node
bool compare_distance(Node* node1, Node* node2) {
    if (node1->distance == node2->distance) {
        return node1->id < node2->id; // Secondary criterion: sort by ID
    }
    return node1->distance < node2->distance;
}

void RobustPrune(Node* node, vector<Node*> possible_neighbours, float a, int max_neighbours) {
    // Add the already existing neighbors to the possible neighbors
    for (Node* n_ptr : node->out_neighbors) {
        possible_neighbours.push_back(n_ptr);
    }

    for (auto it = possible_neighbours.begin(); it != possible_neighbours.end(); ) {
        if ((*it)->id == node->id) {
            it = possible_neighbours.erase(it);  // Erase and move the iterator to the next element
        } else {
            ++it;  // Only increment the iterator if no element was erased
        }
    }

    // Clear node's neighbors
    node->out_neighbors.clear();

    // Calculate distances to possible neighbors
    for (Node* n : possible_neighbours) {
        n->distance = euclidean(node, n);
    }

    // Sort possible neighbors by distance
    sort(possible_neighbours.begin(), possible_neighbours.end(), compare_distance);

    // Select closest neighbors with pruning
    // Iterate through the possible neighbors until empty or reach maxinum neighbors
    while (!possible_neighbours.empty()) {
        Node* closest = possible_neighbours.front();

        // Add the node to the neighbours of the node and remove it from the possible ones
        node->out_neighbors.push_back(closest);
        possible_neighbours.erase(possible_neighbours.begin());

        if (node->out_neighbors.size() == static_cast<size_t>(max_neighbours)) {
            break;
        }

        // Pruning method
        auto it = possible_neighbours.begin();
        while (it != possible_neighbours.end()) {
            float pruning = a * euclidean(closest, *it); 
            if (pruning <= (*it)->distance) {
                it = possible_neighbours.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool isNeighborAlreadyAdded(const std::vector<Node*>& neighbors, Node* neighbor) {
    for (const auto& n : neighbors) {
        if (n == neighbor) {
            return true;
        }
    }
    return false;
}

void robust(void) {
    // Prepare nodes
    vector<vector<float>> node_data = {
        {1.0, 0.0, 0.0, 0.0, 0.0},  // Node 0
        {1.0, 1.0, 1.0, 1.0, 1.0},  // Node 1
        {2.0, 2.0, 2.0, 2.0, 2.0},  // Node 2
        {0.0, 0.0, 3.0, 3.0, 3.0},  // Node 3
        {1.0, 1.0, 4.0, 4.0, 4.0},  // Node 4
        {1.0, 5.0, 5.0, 5.0, 5.0},  // Node 8
        {1.0, 6.0, 6.0, 6.0, 6.0},  // Node 9
        {0.0, 7.0, 7.0, 7.0, 7.0},  // Node 10
        {1.0, 8.0, 8.0, 8.0, 8.0},  // Node 11
        {1.0, 9.0, 9.0, 9.0, 9.0},  // Node 12
        {0.0, 10.0, 10.0, 10.0, 10.0},  // Node 13
        {0.0, 11.0, 11.0, 11.0, 11.0},  // Node 14
        {1.0, 12.0, 12.0, 12.0, 12.0},  // Node 15
        {1.0, 13.0, 13.0, 13.0, 13.0},  // Node 16
        {2.0, 14.0, 14.0, 14.0, 14.0},  // Node 17
        {0.0, 15.0, 15.0, 15.0, 15.0},  // Node 18
        {1.0, 16.0, 16.0, 16.0, 16.0},  // Node 19
        {1.0, 17.0, 17.0, 17.0, 17.0},  // Node 20
        {2.0, 18.0, 18.0, 18.0, 18.0},  // Node 21
        {0.0, 19.0, 19.0, 19.0, 19.0},  // Node 22
        {1.0, 20.0, 20.0, 20.0, 20.0},  // Node 23
        {1.0, 21.0, 21.0, 21.0, 21.0},  // Node 24
        {2.0, 22.0, 22.0, 22.0, 22.0},  // Node 25
        {0.0, 23.0, 23.0, 23.0, 23.0},  // Node 26
        {1.0, 24.0, 24.0, 24.0, 24.0},  // Node 27
        {1.0, 25.0, 25.0, 25.0, 25.0},  // Node 28
        {2.0, 26.0, 26.0, 26.0, 26.0},  // Node 29
        {0.0, 27.0, 27.0, 27.0, 27.0},  // Node 30
        {1.0, 28.0, 28.0, 28.0, 28.0},  // Node 31
        {1.0, 29.0, 29.0, 29.0, 29.0},  // Node 32
        {2.0, 30.0, 30.0, 30.0, 30.0},  // Node 33
        {0.0, 31.0, 31.0, 31.0, 31.0},  // Node 34
        {1.0, 32.0, 32.0, 32.0, 32.0},  // Node 35
        {1.0, 33.0, 33.0, 33.0, 33.0},  // Node 36
        {2.0, 34.0, 34.0, 34.0, 34.0},  // Node 37
        {0.0, 35.0, 35.0, 35.0, 35.0},  // Node 38
        {1.0, 36.0, 36.0, 36.0, 36.0},  // Node 39
        {1.0, 37.0, 37.0, 37.0, 37.0},  // Node 40
        {2.0, 38.0, 38.0, 38.0, 38.0},  // Node 41
        {0.0, 39.0, 39.0, 39.0, 39.0},  // Node 42
        {1.0, 40.0, 40.0, 40.0, 40.0},  // Node 43
        {1.0, 41.0, 41.0, 41.0, 41.0},  // Node 44
        {2.0, 42.0, 42.0, 42.0, 42.0},  // Node 45
        {0.0, 43.0, 43.0, 43.0, 43.0},  // Node 46
        {1.0, 44.0, 44.0, 44.0, 44.0},  // Node 47
        {1.0, 45.0, 45.0, 45.0, 45.0},  // Node 48
        {1.0, 46.0, 46.0, 46.0, 46.0},  // Node 49
        {0.0, 47.0, 47.0, 47.0, 47.0},  // Node 50
        {1.0, 48.0, 48.0, 48.0, 48.0},  // Node 48
        {1.0, 49.0, 49.0, 49.0, 49.0},  // Node 49
        {0.0, 50.0, 50.0, 50.0, 50.0},
    };
    vector<Node*> nodes = createNodesFromVectors(node_data);

    for (int i = 0; i < 15; i++) {
        int num = rand() % 51;

        while (num == 20) {
            num = rand() % 51;
        }

        if (!isNeighborAlreadyAdded(nodes[20]->out_neighbors, nodes[num])) {
            nodes[20]->out_neighbors.push_back(nodes[num]);
        }
    }

    vector<Node*> possible_neighbors;

    for (int i = 5; i < 35; i++) {
        possible_neighbors.push_back(nodes[i]);
    }

    for (uint32_t i = 0; i < nodes[20]->out_neighbors.size(); i++) {
        cout << nodes[20]->out_neighbors[i]->id << endl;
    }

    cout << "Possible neighboirs\n";
    for (uint32_t i = 0; i < possible_neighbors.size(); i++) {
        cout << possible_neighbors[i]->id << endl;
    }

    cout << "ROBUSTTTT\n";

    RobustPrune(nodes[20], possible_neighbors, 1.5, 3);

    for (uint32_t i = 0; i < nodes[20]->out_neighbors.size(); i++) {
        cout << nodes[20]->out_neighbors[i]->id << endl;
    }
}


TEST_LIST = {
    {"robust", robust},

    {NULL, NULL}
};