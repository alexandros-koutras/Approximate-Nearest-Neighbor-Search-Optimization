// FilteredGreedySearch function
#include "../include/vamana.h"

vector<Node*> FilteredGreedySearch(const vector<Node*>& start_nodes, const Node* x_q, unsigned int k, unsigned int list_size, const unordered_set<float>& query_filter) {
    if (start_nodes.empty()) {
        return {}; // Επιστροφή κενής λίστας αν δεν υπάρχουν αρχικοί κόμβοι
    }

    unordered_set<Node*> V; // Σύνολο επισκεφθέντων κόμβων
    vector<Node*> L;        // Λίστα αναζήτησης
    
    // Προσθήκη των αρχικών κόμβων που ικανοποιούν το φίλτρο
    for (Node* s : start_nodes) {
        if (query_filter.find(s->filter) != query_filter.end()) {
            L.push_back(s);
        }
    }

    // Χάρτης για αποθήκευση αποστάσεων
    unordered_map<Node*, float> distances;

    // Βρόχος αναζήτησης
    while (any_of(L.begin(), L.end(), [&](Node* p) { return V.find(p) == V.end(); })) {
        Node* p_star = nullptr;
        float min_distance = numeric_limits<double>::max();

        // Εύρεση του πλησιέστερου μη επισκεφθέντος κόμβου
        for (Node* p : L) {
            if (V.find(p) == V.end()) {
                if (distances.find(p) == distances.end()) {
                    distances[p] = euclidean(p, x_q); // Υπολογισμός απόστασης
                }
                float distance = distances[p];
                if (distance < min_distance) {
                    min_distance = distance;
                    p_star = p;
                }
            }
        }

        if (p_star) {
            V.insert(p_star); // Σήμανση του κόμβου ως επισκεφθέντος

            // Φιλτράρισμα γειτόνων με βάση τις ετικέτες
            for (Node* neighbor : p_star->out_neighbors) {
                if (V.find(neighbor) == V.end() &&
                    query_filter.find(neighbor->filter) != query_filter.end()) {
                    L.push_back(neighbor);
                }
            }


            // Διατήρηση του μεγέθους της λίστας στο όριο list_size
            if (L.size() > list_size) {
                nth_element(L.begin(), L.begin() + list_size, L.end(), [&](Node* a, Node* b) {
                    return euclidean(a, x_q) < euclidean(b, x_q);
                });
                L.resize(list_size);
            }
        } else {
            break; // Τερματισμός αν δεν υπάρχουν επιλέξιμοι κόμβοι
        }
    }

    // Χρήση unordered_set για αφαίρεση διπλοτύπων
    unordered_set<Node*> unique_nodes(L.begin(), L.end());

    // Μετατροπή πίσω σε vector
    L.assign(unique_nodes.begin(), unique_nodes.end());

    // Διατήρηση μόνο των k πλησιέστερων κόμβων
    if (L.size() > k) {
        nth_element(L.begin(), L.begin() + k, L.end(), [&](Node* a, Node* b) {
            return euclidean(a, x_q) < euclidean(b, x_q);
        });
        L.resize(k);
    }

    return L;
}