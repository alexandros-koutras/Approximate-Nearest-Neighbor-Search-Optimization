
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
        //cerr << "Error: Starting node is null." << endl;
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;                                        // Visited nodes
    vector<Node*> L = {s};                                             // Η λίστα αναζήτησης ξεκινά με τον κόμβο s
    // Χάρτης για αποθήκευση αποστάσεων (αποφυγή επαναυπολογισμών)
    unordered_map<Node*, double> distances;

    //  while there is elements in L that are not visited
    while (any_of(L.begin(), L.end(), [&](Node* p){ return V.find(p) == V.end(); })) {

        //find the closest unvisited node to x_q
        Node* p_star = nullptr;
        float min_distance = numeric_limits<float>::max();

        //  traverse the search list L
        // Εύρεση του πλησιέστερου κόμβου που δεν έχει επισκεφθεί
        for (Node* p : L) {
            if (V.find(p) == V.end()) {


                if (distances.find(p) == distances.end()) {
                    distances[p] = euclidean(p, x_q); // Υπολογισμός απόστασης
                }
                double distance = distances[p];


                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }
        if (p_star) {
            //  If we have found p_star, we visit it and add it to the result list
            V.insert(p_star); 

            //  update the search list with p_star's out-neighbors

            for (Node* neighbor : p_star->out_neighbors) {
                //  if they are not visited add them to the search list L
                if (V.find(neighbor) == V.end()) {
                    L.push_back(neighbor);
                }
            }

            // if search list overcome l, we maintain the l closest nodes
            if (L.size() > list_size) {

                //using the function nth_element to sort only the list_size part of the vector 
                nth_element(L.begin(), L.begin() + list_size, L.end(), [&](Node* a, Node* b) {

                        return euclidean(a, x_q) < euclidean(b, x_q);

                    });
                L.resize(list_size); // keep the list_size
            }

        } else {
            // if p_star not found we stop the searching
            break; 
        }
    }


    if (L.size() > k) {
        //using the function nth_element to sort only the list_size part of the vector 
        nth_element(L.begin(), L.begin() + k, L.end(), [&](Node* a, Node* b) {

            return euclidean(a, x_q) < euclidean(b, x_q);

        });
        L.resize(k);
    }
    return L; 
