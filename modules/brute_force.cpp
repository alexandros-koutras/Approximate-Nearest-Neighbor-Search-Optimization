#include "../include/vamana.h"

/// @brief Save knng in binary format (uint32_t) with name "output.bin"
/// @param knn a (N * 100) shape 2-D vector
/// @param path target save path, the output knng should be named as
/// "output.bin" for evaluation
void SaveKNN(const vector<vector<uint32_t>> &knns,const string &path) {
  ofstream ofs(path, ios::out | ios::binary);
  const int K = 100;
  const uint32_t N = knns.size();
  assert(knns.front().size() == K);
  for (unsigned i = 0; i < N; ++i) {
    auto const &knn = knns[i];
    ofs.write(reinterpret_cast<char const *>(&knn[0]), K * sizeof(uint32_t));
  }
  ofs.close();
}

// Brute-force k-NN search function with filter matching
vector<vector<uint32_t>> bruteForceKNNWithFilter(const vector<Node*>& dataset, const vector< Node*>& queries, int k) {
    cout << "inside brute" << endl;
    vector<pair<float, int>> distances;  // Stores pairs of distance and Node ID
    vector<vector<uint32_t>> knn;

    // Calculate distance between query vector and each dataset vector
    
    for (const auto& query : queries) {
        vector<pair<float, int>> distances;  // Reset distances for each query
        vector<uint32_t> knn_ids;  // Reset knn_ids for each query

        for (const auto& node : dataset) {
            if (node->filter == query->filter) {  // Ensure filter matches
                float dist = euclidean(node, query);  // Calculate the Euclidean distance
                distances.push_back({dist, node->id});  // Store the distance and node ID
            }
        }

        // Sort the distances in ascending order
        sort(distances.begin(), distances.end());

        // Retrieve the k nearest neighbors' IDs
        for (int i = 0; i < min(k, (int)distances.size()); ++i) {
            knn_ids.push_back(distances[i].second);  // Store the Node ID of the nearest neighbors
        }

        if (knn_ids.size() < k) {
            cout << "Warning: Found only " << knn_ids.size() << " neighbors for query with ID " << query->id << endl;
            // Fill with placeholder IDs (e.g., UINT32_MAX) if fewer neighbors are found
            while (knn_ids.size() < k) {
                knn_ids.push_back(UINT32_MAX);
            }
        }


        
        knn.push_back(knn_ids);  // Append the k-NN IDs for the current query
    }
    cout << "end of brute" << endl;
    return knn;

}


