#include "../includes/greedysearch.h"

double euclidean(const Node* a, const Node* b) {
    double sum = 0.0;
    for (size_t i = 0; i < a->coords.size(); ++i) {
        sum += pow (a->coords[i] - b->coords[i],2);
    }
    return sqrt(sum);
}


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



// GreedySearch αλγόριθμος
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) {
    if (!s) {
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;  // Visited nodes
    vector<Node*> L = {s};   // Start with the initial node in the search list

    // Priority queue for efficiently finding the closest unvisited node
    using NodeDistPair = pair<double, Node*>;
    priority_queue<NodeDistPair, vector<NodeDistPair>, greater<>> pq;

    // Prepopulate the priority queue with the starting node
    pq.emplace(euclidean(s, x_q), s);

    while (any_of(L.begin(), L.end(), [&](Node* p){ return V.find(p) == V.end(); })) {
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

        // Add out-neighbors of `p_star` to `L`
        for (Node* neighbor : p_star->out_neighbors) {
            if (V.find(neighbor) == V.end()) {
                L.push_back(neighbor);
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

    return L; // Return the `k` closest points from `L`
}

