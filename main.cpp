#include "include/vamana.h"


float computeRecall(const vector<float>& groundTruth, const vector<Node*>& retrievedNeighbors) {
    int truePositiveCount = 0;
    unordered_set<int> retrievedIds;

    // Store retrieved neighbor IDs in a set for fast lookup
    for (Node* node : retrievedNeighbors) {
        retrievedIds.insert(node->id);
    }

    // Count the number of true positives (common neighbors)
    for (int gtId : groundTruth) {
        if (retrievedIds.find(gtId) != retrievedIds.end()) {
            truePositiveCount++;
        }
    }

    if (retrievedNeighbors.size() == 0) {
        return 0;
    }

    // Recall is the fraction of true positive neighbors
    return static_cast<float>(truePositiveCount) / retrievedNeighbors.size();
}

int main(int argc, char* argv[]) {
    if (argc < 11) {
        cerr << "Usage: " << argv[0] << " -i <base.vecs> -q <query.vecs> -g <groundtruth.vecs> -k <k> -l <L> -r <R> -a <a> -s <graph.vecs> -f <stitched_or_filtered> -t <tau>\n";
        return 1;
    }

    string base_file, query_file, groundtruth_file;
    string saved_graph;
    string stitched_or_filtered;
    int k = 0, L = 0, R = 0;
    float a = 0.0;
    unsigned int tau = 0;

    int opt;
    while ((opt = getopt(argc, argv, "i:q:g:k:l:r:a:s:f:t:")) != -1) {
        switch (opt) {
            case 'i':
                base_file = optarg;
                break;
            case 'q':
                query_file = optarg;
                break;
            case 'g':
                groundtruth_file = optarg;
                break;
            case 'k':
                k = stoi(optarg);
                break;
            case 'l':
                L = stoi(optarg);
                break;
            case 'r':
                R = stoi(optarg);
                break;
            case 'a':
                a = stod(optarg);
                break;
            case 's':
                saved_graph = optarg;
                break;
            case 'f':
                stitched_or_filtered = optarg;
                break;
            case 't':
                tau = stoi(optarg);
                break;
            default:
                cerr << "Invalid arguments.\n";
                return 1;
        }
    }

    if (stitched_or_filtered == "stitched") {
        if (saved_graph == "no") {

            vector<vector<float>> nodes_vecs = ReadBin(base_file, 102);
            vector<Node*> nodes = createNodesFromVectors(nodes_vecs);

            if (R <= log2(nodes.size())){
                cerr << "R must be greater than log2(n), so that the graph is well connected"<<endl;
                return 1;
            }

            cout << endl << endl;
            cout << "Base file: " << base_file << endl;
            cout << "Query file: " << query_file << endl;
            cout << "Groundtruth file: " << groundtruth_file << endl;
            cout << "k: " << k << endl;
            cout << "L: " << L << endl;
            cout << "R: " << R << endl;
            cout << "a: " << a << endl;
            cout << endl;
            cout << endl;
            cout << "Now the implementation of the stitched vamana algorithm is starting!" << endl;

            auto start = chrono::high_resolution_clock::now();

            StitchedVamana(nodes, a, 80, 40, R);

            auto end = chrono::high_resolution_clock::now();

            chrono::duration<float> graph_duration = end - start;

            cout << "The filtered vamana graph has been successfully implemented" << endl;
            cout << "Time took to create graph: " << graph_duration.count() << " seconds" << endl;

            vector<vector<float>> queries_vectors = ReadBin(query_file, 104);
            vector<Node*> queries = createQueriesFromVectors(queries_vectors);

            vector<vector<float>> groundtruth = ReadGroundTruth(groundtruth_file);

            float totalRecall = 0.0;
            int queryCount = 0;

            for (size_t i = 0; i < queries.size(); i++) {
                Node* query = queries[i];
                vector<float>& groundTruthForQuery = groundtruth[i];

                int medoid = rand() % nodes.size();

                vector<Node*> nearestNeighbors;
                if (query->distance == 0) {
                    nearestNeighbors = GreedySearch(nodes.at(medoid), query, k, L);
                } else {
                    unordered_set<float> query_filter;
                    query_filter.insert(query->filter);
                    nearestNeighbors = FilteredGreedySearch(nodes, nodes.at(medoid), k, L, query_filter);
                }

                cout << "Nearest neighbors from GreedySearch for query " << query->id << " with type " << query->distance << ": ";
                for (Node* neighbor : nearestNeighbors) {
                    cout << neighbor->id << " ";
                }
                cout << endl;

                cout << "Ground truth neighbors for query " << query->id << ": ";
                for (int gtId : groundTruthForQuery) {
                    cout << gtId << " ";
                }
                cout << endl;

                float recall = computeRecall(groundTruthForQuery, nearestNeighbors);
                totalRecall += recall;
                queryCount++;

                cout << "Recall for query " << query->id << ": " << recall << endl;
                cout << "--------------------------------------------------" << endl;
            }

            float averageRecall = totalRecall / queryCount;
            cout << "Average Recall: " << averageRecall << endl;

            end = chrono::high_resolution_clock::now();
            chrono::duration<float> queries_duration = end - start;

            cout << "The search from the querries is complete!" << endl;
            cout << "Time took to complete the search: " << queries_duration.count() << " seconds" << endl;
            cout << endl;

            chrono::duration<float> total = graph_duration + queries_duration;
            
            cout << endl << "Total time: " << total.count() << " seconds" << endl;

            vector<vector<float>> graph_vector = createVectorFromNodes(nodes);
            SaveVectorToBinary(graph_vector, "graph.bin");

            // Cleanup: free memory
            for (Node* node : nodes) 
                delete node;

            for (Node* node : queries)
                delete node;
        } else {

            auto start = chrono::high_resolution_clock::now();

            vector<vector<float>> nodes_vecs = ReadGraph(saved_graph);
            vector<Node*> nodes = CreateGraph(nodes_vecs);

            cout << endl << endl;
            cout << "Base file: " << base_file << endl;
            cout << "Query file: " << query_file << endl;
            cout << "Groundtruth file: " << groundtruth_file << endl;
            cout << "k: " << k << endl;
            cout << "L: " << L << endl;
            cout << "R: " << R << endl;
            cout << "a: " << a << endl;
            cout << endl;
            cout << endl;
            cout << "Now the implementation of the stitched vamana algorithm is starting!" << endl;

            vector<vector<float>> queries_vectors = ReadBin(query_file, 104);
            vector<Node*> queries = createQueriesFromVectors(queries_vectors);

            vector<vector<float>> groundtruth = ReadGroundTruth(groundtruth_file);

            float totalRecall = 0.0;
            int queryCount = 0;

            for (size_t i = 0; i < queries.size(); i++) {
                Node* query = queries[i];
                vector<float>& groundTruthForQuery = groundtruth[i];

                int medoid = rand() % nodes.size();

                vector<Node*> nearestNeighbors;
                if (query->distance == 0) {
                    nearestNeighbors = GreedySearch(nodes.at(medoid), query, k, L);
                } else {
                    unordered_set<float> query_filter;
                    query_filter.insert(query->filter);
                    nearestNeighbors = FilteredGreedySearch(nodes, nodes.at(medoid), k, L, query_filter);
                }

                cout << "Nearest neighbors from GreedySearch for query " << query->id << " with type " << query->distance << ": ";
                for (Node* neighbor : nearestNeighbors) {
                    cout << neighbor->id << " ";
                }
                cout << endl;

                cout << "Ground truth neighbors for query " << query->id << ": ";
                for (int gtId : groundTruthForQuery) {
                    cout << gtId << " ";
                }
                cout << endl;

                float recall = computeRecall(groundTruthForQuery, nearestNeighbors);
                totalRecall += recall;
                queryCount++;

                cout << "Recall for query " << query->id << ": " << recall << endl;
                cout << "--------------------------------------------------" << endl;
            }

            float averageRecall = totalRecall / queryCount;
            cout << "Average Recall: " << averageRecall << endl;

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<float> queries_duration = end - start;

            cout << "The search from the querries is complete!" << endl;
            cout << "Time took to complete the search: " << queries_duration.count() << " seconds" << endl;
            cout << endl;

            // Cleanup: free memory
            for (Node* node : nodes) 
                delete node;

            for (Node* node : queries)
                delete node;
        }
    } else {
        if (saved_graph == "no") {

            vector<vector<float>> nodes_vecs = ReadBin(base_file, 102);
            vector<Node*> nodes = createNodesFromVectors(nodes_vecs);

            if (R <= log2(nodes.size())){
                cerr << "R must be greater than log2(n), so that the graph is well connected"<<endl;
                return 1;
            }

            cout << endl << endl;
            cout << "Base file: " << base_file << endl;
            cout << "Query file: " << query_file << endl;
            cout << "Groundtruth file: " << groundtruth_file << endl;
            cout << "k: " << k << endl;
            cout << "L: " << L << endl;
            cout << "R: " << R << endl;
            cout << "a: " << a << endl;
            cout << endl;
            cout << endl;
            cout << "Now the implementation of the stitched vamana algorithm is starting!" << endl;

            auto start = chrono::high_resolution_clock::now();

            DirectedGraph d = FilteredVamana(nodes, k, L, R, a, tau);

            auto end = chrono::high_resolution_clock::now();

            chrono::duration<float> graph_duration = end - start;

            cout << "The filtered vamana graph has been successfully implemented" << endl;
            cout << "Time took to create graph: " << graph_duration.count() << " seconds" << endl;

            vector<vector<float>> queries_vectors = ReadBin(query_file, 104);
            vector<Node*> queries = createQueriesFromVectors(queries_vectors);

            vector<vector<float>> groundtruth = ReadGroundTruth(groundtruth_file);

            float totalRecall = 0.0;
            int queryCount = 0;

            for (size_t i = 0; i < queries.size(); i++) {
                Node* query = queries[i];
                vector<float>& groundTruthForQuery = groundtruth[i];

                int medoid = rand() % nodes.size();

                vector<Node*> nearestNeighbors;
                if (query->distance == 0) {
                    nearestNeighbors = GreedySearch(nodes.at(medoid), query, k, L);
                } else {
                    unordered_set<float> query_filter;
                    query_filter.insert(query->filter);
                    nearestNeighbors = FilteredGreedySearch(nodes, nodes.at(medoid), k, L, query_filter);
                }

                cout << "Nearest neighbors from GreedySearch for query " << query->id << " with type " << query->distance << ": ";
                for (Node* neighbor : nearestNeighbors) {
                    cout << neighbor->id << " ";
                }
                cout << endl;

                cout << "Ground truth neighbors for query " << query->id << ": ";
                for (int gtId : groundTruthForQuery) {
                    cout << gtId << " ";
                }
                cout << endl;

                float recall = computeRecall(groundTruthForQuery, nearestNeighbors);
                totalRecall += recall;
                queryCount++;

                cout << "Recall for query " << query->id << ": " << recall << endl;
                cout << "--------------------------------------------------" << endl;
            }

            float averageRecall = totalRecall / queryCount;
            cout << "Average Recall: " << averageRecall << endl;

            end = chrono::high_resolution_clock::now();
            chrono::duration<float> queries_duration = end - start;

            cout << "The search from the querries is complete!" << endl;
            cout << "Time took to complete the search: " << queries_duration.count() << " seconds" << endl;
            cout << endl;

            chrono::duration<float> total = graph_duration + queries_duration;
            
            cout << endl << "Total time: " << total.count() << " seconds" << endl;

            vector<vector<float>> graph_vector = createVectorFromNodes(nodes);
            SaveVectorToBinary(graph_vector, "graph.bin");

            // Cleanup: free memory
            for (Node* node : nodes) 
                delete node;

            for (Node* node : queries)
                delete node;
        } else {

            auto start = chrono::high_resolution_clock::now();

            vector<vector<float>> nodes_vecs = ReadBin(base_file, 102);
            vector<Node*> nodes = createNodesFromVectors(nodes_vecs);

            cout << endl << endl;
            cout << "Base file: " << base_file << endl;
            cout << "Query file: " << query_file << endl;
            cout << "Groundtruth file: " << groundtruth_file << endl;
            cout << "k: " << k << endl;
            cout << "L: " << L << endl;
            cout << "R: " << R << endl;
            cout << "a: " << a << endl;
            cout << endl;
            cout << endl;
            cout << "Now the implementation of the stitched vamana algorithm is starting!" << endl;

            vector<vector<float>> queries_vectors = ReadBin(query_file, 104);
            vector<Node*> queries = createQueriesFromVectors(queries_vectors);

            vector<vector<float>> groundtruth = ReadGroundTruth(groundtruth_file);

            float totalRecall = 0.0;
            int queryCount = 0;

            for (size_t i = 0; i < queries.size(); i++) {
                Node* query = queries[i];
                vector<float>& groundTruthForQuery = groundtruth[i];

                int medoid = rand() % nodes.size();

                vector<Node*> nearestNeighbors;
                if (query->distance == 0) {
                    nearestNeighbors = GreedySearch(nodes.at(medoid), query, k, L);
                } else {
                    unordered_set<float> query_filter;
                    query_filter.insert(query->filter);
                    nearestNeighbors = FilteredGreedySearch(nodes, nodes.at(medoid), k, L, query_filter);
                }

                cout << "Nearest neighbors from GreedySearch for query " << query->id << " with type " << query->distance << ": ";
                for (Node* neighbor : nearestNeighbors) {
                    cout << neighbor->id << " ";
                }
                cout << endl;

                cout << "Ground truth neighbors for query " << query->id << ": ";
                for (int gtId : groundTruthForQuery) {
                    cout << gtId << " ";
                }
                cout << endl;

                float recall = computeRecall(groundTruthForQuery, nearestNeighbors);
                totalRecall += recall;
                queryCount++;

                cout << "Recall for query " << query->id << ": " << recall << endl;
                cout << "--------------------------------------------------" << endl;
            }

            float averageRecall = totalRecall / queryCount;
            cout << "Average Recall: " << averageRecall << endl;

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<float> queries_duration = end - start;

            cout << "The search from the querries is complete!" << endl;
            cout << "Time took to complete the search: " << queries_duration.count() << " seconds" << endl;
            cout << endl;

            // Cleanup: free memory
            for (Node* node : nodes) 
                delete node;

            for (Node* node : queries)
                delete node;
        }
    }

}