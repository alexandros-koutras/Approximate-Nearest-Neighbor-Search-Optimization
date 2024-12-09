#include "include/vamana.h"
#include <getopt.h>
#include <chrono>

int main(int argc, char* argv[]) {
    // if (argc < 8) {
    //     cerr << "Usage: " << argv[0] << " -i <base.vecs> -q <query.vecs>  -k <k> \n";
    //     return 1;
    // }
    string base_file;
    string query_file;
    int k = 0;
    int opt;
    while ((opt = getopt(argc, argv, "i:q:k:")) != -1) {
        switch (opt) {
            case 'i':
                base_file = optarg;
                break;
            case 'q':
                query_file = optarg;
                break;
            case 'k':
                k = stoi(optarg);
                break;
            default:
                cerr << "Invalid arguments.\n";
                return 1;
        }
    }

            vector<vector<float>> data = ReadBin(base_file, 102);

            vector<Node*> nodes = createNodesFromVectors(data);

            vector<vector<float>> queries = ReadBin(query_file, 104);

            vector<Node*> query_nodes  = createQueriesFromVectors(data);

            int n = nodes.size();

            std::cout << endl << endl;
            std::cout << "Base file: " << base_file << endl;
            std::cout << "Query file: " <<query_file << endl;
            std::cout << "k: " << k << endl;
            std::cout << endl;
            std::cout << "Now the implementation of the bruteForce algorithm is starting!" << endl;

            SaveKNN(bruteForceKNNWithFilter(nodes, query_nodes,k));
          
            // Cleanup: free memory
            for (Node* node : nodes) 
                delete node;

            for (Node* node : query_nodes)
                delete node;
        
    return 0;
}