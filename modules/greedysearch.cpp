#include "../include/vamana.h"

// GreedySearch αλγόριθμος
vector<Node*> GreedySearch(Node* s, const Node* x_q, unsigned int k, unsigned int list_size) {
    if (!s) {
        return {}; // Return an empty result if the starting node is null
    }

    unordered_set<Node*> V;  // Visited nodes
    vector<Node*> L = {s};   // Η λίστα αναζήτησης ξεκινά με τον κόμβο s
    unordered_map<Node*, double> distances; // Χάρτης αποστάσεων (για αποφυγή επαναυπολογισμών)

    // Ενώ υπάρχουν κόμβοι στη λίστα L που δεν έχουν επισκεφθεί
    while (any_of(L.begin(), L.end(), [&](Node* p){ return V.find(p) == V.end(); })) {
        Node* p_star = nullptr;
        double min_distance = numeric_limits<double>::max();

        // Εύρεση του πλησιέστερου μη επισκέψιμου κόμβου
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
            V.insert(p_star); // Σημείωση του p_star ως επισκέψιμου

            // Προσθήκη των out-neighbors του p_star στη λίστα L
            for (Node* neighbor : p_star->out_neighbors) {
                if (V.find(neighbor) == V.end()) {
                    L.push_back(neighbor);
                }
            }

            // Αν η λίστα L ξεπερνά το list_size, κρατάμε μόνο τους list_size πιο κοντινούς κόμβους
            if (L.size() > list_size) {
                nth_element(L.begin(), L.begin() + list_size, L.end(), [&](Node* a, Node* b) {
                    return euclidean(a, x_q) < euclidean(b, x_q);
                });
                L.resize(list_size);
            }
        } else {
            break; // Τερματισμός αν δεν υπάρχει πλησιέστερος κόμβος
        }
    }

    // Αφαίρεση διπλοτύπων από τη λίστα L
    unordered_set<Node*> unique_nodes(L.begin(), L.end());
    L.assign(unique_nodes.begin(), unique_nodes.end());

    // Περιορισμός της λίστας L στους k πιο κοντινούς γείτονες
    if (L.size() > k) {
        nth_element(L.begin(), L.begin() + k, L.end(), [&](Node* a, Node* b) {
            return euclidean(a, x_q) < euclidean(b, x_q);
        });
        L.resize(k);
    }

    return L;
}
