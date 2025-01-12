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

vector<Node*> createQueriesFromVectors(const vector<vector<float>>& vectors) {
    vector<Node*> nodes;

    for (size_t i = 0; i < vectors.size(); ++i) {
        Node* newNode = new Node;
        newNode->id = i;  // Use index as the ID
        newNode->distance = vectors[i].at(0);
        if (newNode->distance == 2 || newNode->distance == 3) {
            delete newNode;
            continue;
        } 
        newNode->filter = vectors[i].at(1);
        newNode->coords.assign(vectors[i].begin() + 4, vectors[i].end());  // Copy coordinates into the Node
        nodes.push_back(newNode);  // Add the Node to the list
    }

    return nodes;
}

// This is the euklideian method to calculate the distance of 2 nodes
float euclidean(const Node* a, const Node* b) {
    float sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        float diff = a->coords[i] - b->coords[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Compare function for 2 nodes based to their distance to another common node
bool compare_distance(Node* node1, Node* node2) {
    if (node1->distance == node2->distance) {
        return node1->id < node2->id; // Secondary criterion: sort by ID
    }
    return node1->distance < node2->distance;
}

vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) {
    if (!s) {
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;  // Visited nodes
    unordered_set<Node*> unique_nodes; // Ensure unique nodes in the result
    vector<Node*> L = {s};   // Start with the initial node in the search list

    // Priority queue for efficiently finding the closest unvisited node
    using NodeDistPair = pair<double, Node*>;
    priority_queue<NodeDistPair, vector<NodeDistPair>, greater<>> pq;

    // Prepopulate the priority queue with the starting node
    pq.emplace(euclidean(s, x_q), s);

    while (any_of(L.begin(), L.end(), [&](Node* p) { return V.find(p) == V.end(); })) {
        // Find the closest unvisited node
        auto top = pq.top();
        Node* p_star = top.second;
        pq.pop();

        // Skip if already visited
        if (V.find(p_star) != V.end()) {
            continue;
        }

        // Mark the node as visited
        V.insert(p_star);

        // Add out-neighbors of `p_star` to `L` if not visited
        for (Node* neighbor : p_star->out_neighbors) {
            if (V.find(neighbor) == V.end() && unique_nodes.find(neighbor) == unique_nodes.end()) {
                L.push_back(neighbor);
                unique_nodes.insert(neighbor); // Mark as unique
                pq.emplace(euclidean(neighbor, x_q), neighbor); // Add to priority queue
            }
        }

        // If L exceeds the allowed size, retain only the closest `list_size` points
        if (L.size() > list_size) {
            nth_element(L.begin(), L.begin() + list_size, L.end(),
                        [&](Node* a, Node* b) {
                            return euclidean(a, x_q) < euclidean(b, x_q);
                        });
            L.resize(list_size);
        }
    }

    // Extract the closest `k` nodes from L
    if (L.size() > k) {
        nth_element(L.begin(), L.begin() + k, L.end(),
                    [&](Node* a, Node* b) {
                        return euclidean(a, x_q) < euclidean(b, x_q);
                    });
        L.resize(k);
    }

    return L; // Return the `k` closest unique points from `L`
}

void test_brute_force_basic_case() {
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
        // Repeat similar pattern until Node 99
    };
    vector<Node*> nodes = createNodesFromVectors(node_data);

    // Prepare queries
    vector<vector<float>> query_data = {
        {0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0},  // Query 0
        {1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0},  // Query 1
        {0.0, -1.0, 15.0, 15.0, 15.0, 15.0, 15.0}, // Query 3
        {1.0, 1.0, 3.0, 3.0, 3.0, 3.0, 3.0} // Query 2
    };
    vector<Node*> queries = createQueriesFromVectors(query_data);

    nodes[0]->out_neighbors.push_back(nodes[1]);
    nodes[0]->out_neighbors.push_back(nodes[5]);
    nodes[0]->out_neighbors.push_back(nodes[2]);
    nodes[0]->out_neighbors.push_back(nodes[3]);
    nodes[0]->out_neighbors.push_back(nodes[7]);
    nodes[1]->out_neighbors.push_back(nodes[3]);
    nodes[1]->out_neighbors.push_back(nodes[4]);
    nodes[1]->out_neighbors.push_back(nodes[2]);
    nodes[1]->out_neighbors.push_back(nodes[6]);
    nodes[2]->out_neighbors.push_back(nodes[5]);
    nodes[2]->out_neighbors.push_back(nodes[8]);
    nodes[2]->out_neighbors.push_back(nodes[9]);
    nodes[3]->out_neighbors.push_back(nodes[6]);
    nodes[3]->out_neighbors.push_back(nodes[4]);
    nodes[3]->out_neighbors.push_back(nodes[5]);
    nodes[4]->out_neighbors.push_back(nodes[7]);
    nodes[4]->out_neighbors.push_back(nodes[2]);
    nodes[4]->out_neighbors.push_back(nodes[0]);
    nodes[5]->out_neighbors.push_back(nodes[8]);
    nodes[5]->out_neighbors.push_back(nodes[1]);
    nodes[5]->out_neighbors.push_back(nodes[6]);
    nodes[6]->out_neighbors.push_back(nodes[9]);
    nodes[6]->out_neighbors.push_back(nodes[2]);
    nodes[6]->out_neighbors.push_back(nodes[8]);
    nodes[7]->out_neighbors.push_back(nodes[0]);
    nodes[7]->out_neighbors.push_back(nodes[3]);
    nodes[7]->out_neighbors.push_back(nodes[9]);
    nodes[8]->out_neighbors.push_back(nodes[0]);
    nodes[8]->out_neighbors.push_back(nodes[5]);
    nodes[9]->out_neighbors.push_back(nodes[4]);
    nodes[9]->out_neighbors.push_back(nodes[6]);

    cout << "OK\n";

    nodes[10]->out_neighbors.push_back(nodes[20]);
    nodes[10]->out_neighbors.push_back(nodes[25]);
    nodes[10]->out_neighbors.push_back(nodes[22]);
    nodes[10]->out_neighbors.push_back(nodes[23]);
    nodes[10]->out_neighbors.push_back(nodes[27]);
    nodes[11]->out_neighbors.push_back(nodes[33]);
    nodes[11]->out_neighbors.push_back(nodes[44]);
    nodes[11]->out_neighbors.push_back(nodes[42]);
    nodes[11]->out_neighbors.push_back(nodes[46]);
    nodes[12]->out_neighbors.push_back(nodes[15]);
    nodes[12]->out_neighbors.push_back(nodes[28]);
    nodes[12]->out_neighbors.push_back(nodes[42]);
    nodes[13]->out_neighbors.push_back(nodes[46]);
    nodes[13]->out_neighbors.push_back(nodes[34]);
    nodes[13]->out_neighbors.push_back(nodes[25]);
    nodes[14]->out_neighbors.push_back(nodes[7]);
    nodes[14]->out_neighbors.push_back(nodes[12]);
    nodes[14]->out_neighbors.push_back(nodes[0]);
    nodes[15]->out_neighbors.push_back(nodes[8]);
    nodes[15]->out_neighbors.push_back(nodes[1]);
    nodes[15]->out_neighbors.push_back(nodes[26]);
    nodes[16]->out_neighbors.push_back(nodes[9]);
    nodes[16]->out_neighbors.push_back(nodes[32]);
    nodes[16]->out_neighbors.push_back(nodes[8]);
    nodes[17]->out_neighbors.push_back(nodes[0]);
    nodes[17]->out_neighbors.push_back(nodes[3]);
    nodes[17]->out_neighbors.push_back(nodes[4]);
    nodes[18]->out_neighbors.push_back(nodes[40]);
    nodes[18]->out_neighbors.push_back(nodes[25]);
    nodes[19]->out_neighbors.push_back(nodes[42]);
    nodes[19]->out_neighbors.push_back(nodes[6]);

    cout << "OK\n";

    nodes[20]->out_neighbors.push_back(nodes[11]);
    nodes[20]->out_neighbors.push_back(nodes[25]);
    nodes[20]->out_neighbors.push_back(nodes[32]);
    nodes[20]->out_neighbors.push_back(nodes[43]);
    nodes[20]->out_neighbors.push_back(nodes[37]);
    nodes[21]->out_neighbors.push_back(nodes[23]);
    nodes[21]->out_neighbors.push_back(nodes[14]);
    nodes[21]->out_neighbors.push_back(nodes[2]);
    nodes[21]->out_neighbors.push_back(nodes[6]);
    nodes[22]->out_neighbors.push_back(nodes[5]);
    nodes[22]->out_neighbors.push_back(nodes[8]);
    nodes[22]->out_neighbors.push_back(nodes[19]);
    nodes[23]->out_neighbors.push_back(nodes[36]);
    nodes[23]->out_neighbors.push_back(nodes[24]);
    nodes[23]->out_neighbors.push_back(nodes[25]);
    nodes[24]->out_neighbors.push_back(nodes[27]);
    nodes[24]->out_neighbors.push_back(nodes[32]);
    nodes[24]->out_neighbors.push_back(nodes[40]);
    nodes[25]->out_neighbors.push_back(nodes[18]);
    nodes[25]->out_neighbors.push_back(nodes[11]);
    nodes[25]->out_neighbors.push_back(nodes[6]);
    nodes[26]->out_neighbors.push_back(nodes[19]);
    nodes[26]->out_neighbors.push_back(nodes[22]);
    nodes[26]->out_neighbors.push_back(nodes[28]);
    nodes[27]->out_neighbors.push_back(nodes[30]);
    nodes[27]->out_neighbors.push_back(nodes[43]);
    nodes[27]->out_neighbors.push_back(nodes[9]);
    nodes[28]->out_neighbors.push_back(nodes[0]);
    nodes[28]->out_neighbors.push_back(nodes[35]);
    nodes[29]->out_neighbors.push_back(nodes[4]);
    nodes[29]->out_neighbors.push_back(nodes[6]);

    cout << "OK\n";

    nodes[30]->out_neighbors.push_back(nodes[10]);
    nodes[30]->out_neighbors.push_back(nodes[1]);
    nodes[30]->out_neighbors.push_back(nodes[20]);
    nodes[30]->out_neighbors.push_back(nodes[30]);
    nodes[30]->out_neighbors.push_back(nodes[27]);
    nodes[31]->out_neighbors.push_back(nodes[23]);
    nodes[31]->out_neighbors.push_back(nodes[24]);
    nodes[31]->out_neighbors.push_back(nodes[22]);
    nodes[31]->out_neighbors.push_back(nodes[26]);
    nodes[32]->out_neighbors.push_back(nodes[15]);
    nodes[32]->out_neighbors.push_back(nodes[28]);
    nodes[32]->out_neighbors.push_back(nodes[9]);
    nodes[33]->out_neighbors.push_back(nodes[6]);
    nodes[33]->out_neighbors.push_back(nodes[4]);
    nodes[33]->out_neighbors.push_back(nodes[5]);
    nodes[34]->out_neighbors.push_back(nodes[17]);
    nodes[34]->out_neighbors.push_back(nodes[2]);
    nodes[34]->out_neighbors.push_back(nodes[0]);
    nodes[35]->out_neighbors.push_back(nodes[28]);
    nodes[35]->out_neighbors.push_back(nodes[1]);
    nodes[35]->out_neighbors.push_back(nodes[26]);
    nodes[36]->out_neighbors.push_back(nodes[9]);
    nodes[36]->out_neighbors.push_back(nodes[23]);
    nodes[36]->out_neighbors.push_back(nodes[8]);
    nodes[37]->out_neighbors.push_back(nodes[0]);
    nodes[37]->out_neighbors.push_back(nodes[33]);
    nodes[37]->out_neighbors.push_back(nodes[9]);
    nodes[38]->out_neighbors.push_back(nodes[0]);
    nodes[38]->out_neighbors.push_back(nodes[35]);
    nodes[39]->out_neighbors.push_back(nodes[4]);
    nodes[39]->out_neighbors.push_back(nodes[45]);

    cout << "OK\n";

    nodes[40]->out_neighbors.push_back(nodes[11]);
    nodes[40]->out_neighbors.push_back(nodes[15]);
    nodes[40]->out_neighbors.push_back(nodes[12]);
    nodes[40]->out_neighbors.push_back(nodes[23]);
    nodes[40]->out_neighbors.push_back(nodes[37]);
    nodes[41]->out_neighbors.push_back(nodes[43]);
    nodes[41]->out_neighbors.push_back(nodes[34]);
    nodes[41]->out_neighbors.push_back(nodes[22]);
    nodes[41]->out_neighbors.push_back(nodes[16]);
    nodes[42]->out_neighbors.push_back(nodes[5]);
    nodes[42]->out_neighbors.push_back(nodes[28]);
    nodes[42]->out_neighbors.push_back(nodes[39]);
    nodes[43]->out_neighbors.push_back(nodes[46]);
    nodes[43]->out_neighbors.push_back(nodes[34]);
    nodes[43]->out_neighbors.push_back(nodes[25]);
    nodes[44]->out_neighbors.push_back(nodes[17]);
    nodes[44]->out_neighbors.push_back(nodes[2]);
    nodes[44]->out_neighbors.push_back(nodes[10]);
    nodes[45]->out_neighbors.push_back(nodes[28]);
    nodes[45]->out_neighbors.push_back(nodes[31]);
    nodes[45]->out_neighbors.push_back(nodes[46]);
    nodes[46]->out_neighbors.push_back(nodes[39]);
    nodes[46]->out_neighbors.push_back(nodes[22]);
    nodes[46]->out_neighbors.push_back(nodes[18]);
    nodes[47]->out_neighbors.push_back(nodes[0]);
    nodes[47]->out_neighbors.push_back(nodes[13]);
    nodes[47]->out_neighbors.push_back(nodes[29]);

    cout << "OK\n";

    // Perform brute-force search
    int medoid = rand() % nodes.size();
    vector<Node*> greedy = GreedySearch(nodes[15], queries[2], 10, 15);

    cout << "How many were returned " << greedy.size() << endl;

    for (int i = 0; i < 10; i++) {
        cout << greedy[i]->id << endl;
    }

    // Validate results
    // For Query 0 (distance == 0), closest to all nodes, sorted by ID
    // TEST_CHECK(greedy[0].size() == 10);
    // TEST_CHECK(greedy[0][0] == 0);  // Closest
    // TEST_CHECK(greedy[0][1] == 1);
    // TEST_CHECK(greedy[0][2] == 2);
    // TEST_CHECK(greedy[0][3] == 3);
    // TEST_CHECK(greedy[0][4] == 4);
    // TEST_CHECK(greedy[0][5] == 5);
    // TEST_CHECK(greedy[0][6] == 6);
    // TEST_CHECK(greedy[0][7] == 7);
    // TEST_CHECK(greedy[0][8] == 8);
    // TEST_CHECK(greedy[0][9] == 9);  // Furthest

    // For Query 1 (distance != 0, filter == 1.0), only Node 2 matches
    // TEST_CHECK(greedy[1].size() == 10);
    // TEST_CHECK(greedy[1][0] == 1);  // Check that Node 2 is the closest
    // TEST_CHECK(greedy[1][1] == 0);
    // TEST_CHECK(greedy[1][2] == 4);
    // TEST_CHECK(greedy[1][3] == 5);
    // TEST_CHECK(greedy[1][4] == 6);
    // TEST_CHECK(greedy[1][5] == 8);
    // TEST_CHECK(greedy[1][6] == 9);
    // TEST_CHECK(greedy[1][7] == 12);
    // TEST_CHECK(greedy[1][8] == 13);
    // TEST_CHECK(greedy[1][9] == 16);

    TEST_CHECK(greedy.size() == 10);
    TEST_CHECK(greedy[0] == nodes[15]);  // Check that Node 2 is the closest
    TEST_CHECK(greedy[1] == nodes[14]);
    TEST_CHECK(greedy[2] == nodes[16]);
    TEST_CHECK(greedy[3] == nodes[13]);
    TEST_CHECK(greedy[4] == nodes[17]);
    TEST_CHECK(greedy[5] == nodes[12]);
    TEST_CHECK(greedy[6] == nodes[18]);
    TEST_CHECK(greedy[7] == nodes[11]);
    TEST_CHECK(greedy[8] == nodes[19]);
    TEST_CHECK(greedy[9] == nodes[10]);

    // Clean up
    for (Node* node : nodes) delete node;
    for (Node* query : queries) delete query;
}

TEST_LIST = {
    {"test_brute_force_basic_case", test_brute_force_basic_case},

    {NULL, NULL}
};
