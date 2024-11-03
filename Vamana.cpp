#include "functions.h"

//random R-regulated directed graph
void initializeRandomGraph(vector<Node*>& nodes, int R) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (Node* node : nodes) {
        unordered_set<int> neighbors;
        
        while (neighbors.size() < R) {
            int random_index = rand() % nodes.size();
            if (random_index != node->id) {
                Node* neighbor = nodes[random_index];
                node->add_neighbour(neighbor);
                neighbors.insert(random_index);
            }
        }
    }

}

void VamanaIndexingAlgorithm(vector<Node*>& nodes, int L, int R, double a,int n) {
    // Step 1: Initialize a random R-regular directed graph
    initializeRandomGraph(nodes, R);

    // Step 2: Find the medoid 's' of the dataset (e.g., first node or central node)
    if (n==0)return;//empty
        int medoid=0;
        float min_dist=numeric_limits<float>::max();
        for (int i=0;i<n;i++){
            float total_dist=0;
            for(int j=0;j<n;j++){
                if(i!=j){
                    total_dist+=euclidean(nodes[i],nodes[j]);
                }
            }
            if (total_dist<min_dist){
                min_dist=total_dist;
                medoid=i;
            }
        }
    Node* s=nodes[medoid];

    // Step 3: Iterate through the dataset in a random order
    vector<int> perm(nodes.size());
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), mt19937(random_device{}()));

    for (int i : perm) {
        Node* p = nodes[i];
        
        // Run GreedySearch to find the visited set V_p
        vector<Node*> V_p = GreedySearch(s, p, 1, L);

        // Run RobustPrune on p with V_p, a, and R
        RobustPrune(p, V_p, a, R);

        // Add reverse edges to enforce bidirectional links
        for (Node* neighbor : p->out_neighbors) {
            neighbor->add_neighbour(p);

            // If neighbor exceeds max out-degree R, apply RobustPrune
            if (neighbor->out_neighbors.size() > static_cast<size_t>(R)) {
                RobustPrune(neighbor, neighbor->out_neighbors, a, R);
            }
        }
    }
}

// class VamanaGraph{
//     vector<Node*> nodes;
//     Node* query;
//     int n;//number nodes
//     int R; //maximum number of neighbors a node can have
//     float a; //pruning factor
//     int L; //list size
//     int k;

// public:
//     VamanaGraph(vector<Node*>& nodes,const vector<Node*>& query, int R, float a, int L,int n, int k)
//         :nodes(nodes),query(query), R(R),a(a),L(L),n(n),k(k){
//         // nodes.resize(n);
//         // for(int i=0;i<n;i++){
//         //     nodes[i]=Node(i);//each node has an id
//         // }
//     }

//     void random_graph(){//random graphs with random neighbors
//         srand(static_cast<unsigned int>(time(nullptr)));
//         for (Node* node : nodes){
//             unordered_set<Node*> neighbors;
//             while (neighbors.size()<R){
//                 Node* potential_neighbor=nodes[rand()%n];
//                 if (potential_neighbor!=node)neighbors.insert(potential_neighbor);//prevents a node from being its own neighbor
//             }
//             node->out_neighbors.assign(neighbors.begin(),neighbors.end());
//         }
//     }

//     void VamanaIndexingAlgorithm(){
//         srand(static_cast<unsigned int>(time(nullptr)));
//         vector<int> permutation(n);//list of all indices
//         for(int i=0;i<n;i++){//populate the list so contains indices from 0 to n-1
//             permutation[i]=i;
//         }
//         //fisher-yates shuffles//random_graph()
//         for(int i=n-1;i>0,i--){
//             int j=rand()%(i+1);//random num from 0 to i
//             swap(permutation[i],permutation[j]);
//         }
//         int medoid=find_medoid();//implementation to find initial point
//         for(int i:permutation){
//             auto [L,V]=GreedySearch(nodes[0], query, k, L);
//             RobustPrune(nodes[i],V,a,R);
//             for (int neighbor:nodes[i].out_neighbors){
//                 if(nodes[neighbor].out_neighbors.size()<=R){
//                     nodes[neighbor].out_neighbors.push_back(i);
//                 }else{
//                     RobustPrune(neighbor,/*current neighbors*/,a,R);
//                 }
//             }
//         }
//     }

//     int find_medoid(){
//         if (n==0)return -1;//empty
//         int medoid=0;
//         float min_dist=numeric_limits<float>::max();
//         for (int i=0;i<n;i++){
//             float total_dist=0;
//             for(int j=0;j<n;j++){
//                 if(i!=j){
//                     total_dist+=euclidean(nodes[i],nodes[j]);
//                 }
//             }
//             if (total_dist<min_dist){
//                 min_dist=total_dist;
//                 medoid=i;
//             }
//         }
//         return medoid;
//     }
// };

// int main(int argc, char* argv[]){
//     if(argc<7){
//         cerr<<"Usage: " << argv[0] << " <base.fvecs> <query.fvecs> <k> <L>" << endl;
//         return 1;    
//     }
//     string base_file=argv[1];
//     vector<Node*> nodes = load_vecs(base_file);

//     string query_file = argv[2];
//     vector<Node*> query_nodes = load_vecs(query_file);
//     int k = stoi(argv[3]);  // number of closest nodes to find
//     int R=atoi(argv[4]);//maximum number of neighbors a node can have
//     float a=atof(argv[5]);//pruning factor
//     int L=atoi(argv[6]);//list size
//     int n=nodes.size();
//     //well connected
//     if(R<=log2(n)){
//         cerr<<"R must be greater than log2(n), so that the graph is well connected"<<endl;
//         return 1;
//     }

//     VamanaGraph graph(nodes,query_nodes, R, a, L,n, k);//k ορισμα, query
//     //graph.random_graph();//tha to kaleso sto algorithm
//     graph.VamanaIndexingAlgorithm();

//     for (Node* node : nodes) delete node;
//     for (Node* query : query_nodes) delete query;

//     return 0;
// }