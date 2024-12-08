#include "../include/vamana.h"

// Find the centroid of a cluster
Node* findCentroid(const vector<Node*>& cluster) {
    if (cluster.empty()) {
        throw invalid_argument("Cluster is empty, cannot find centroid");
    }

    size_t dimensions = cluster[0]->coords.size(); // Number of dimensions
    Node* centroid = new Node; // Centroid node initialization
    centroid->coords.resize(dimensions, 0.0);

    // Sum up the coordinates of each node
    for (const Node* point : cluster) {
        for (size_t i = 0; i < dimensions; ++i) {
            centroid->coords[i] += point->coords[i];
        }
    }

    // Divide each coordinate sum by the number of points to get the centroid
    for (float& value : centroid->coords) {
        value /= cluster.size();
    }

    return centroid;
}

// K-means clustering
vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations) {
    if (k <= 0 || k > (int)nodes.size()) {
        throw invalid_argument("Invalid number of clusters");
    }

    vector<Node*> centroids;
    vector<vector<Node*>> clusters(k); // Vector of clusters, one for each centroid

    // Initialize random centroids
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(new Node(*nodes[rand() % nodes.size()])); // Create new nodes for centroids
    }

    // K-means iterations
    for (int iter = 0; iter < maxIterations; ++iter) {
        // Clear clusters for the current iteration
        for (auto& cluster : clusters) {
            cluster.clear();
        }

        // Assign each node to the nearest centroid
        for (const Node* node : nodes) {
            int closestCluster = 0;
            float minDist = euclidean(node, centroids[0]); // Distance to the first centroid
            for (int j = 1; j < k; ++j) {
                float dist = euclidean(node, centroids[j]);
                if (dist < minDist) { // If a nearer centroid is found
                    minDist = dist;
                    closestCluster = j;
                }
            }
            clusters[closestCluster].push_back(const_cast<Node*>(node));
        }

        // Recalculate centroids
        bool centroidsChanged = false;
        for (int i = 0; i < k; ++i) {
            if (clusters[i].empty()) continue; // Skip empty clusters

            Node* newCentroid = findCentroid(clusters[i]);
            if (euclidean(newCentroid, centroids[i]) > 1e-4) { // Check if the centroid has changed significantly
                centroidsChanged = true;
            }

            delete centroids[i]; // Clean up old centroid
            centroids[i] = newCentroid;
        }

        // If centroids don't change, break early
        if (!centroidsChanged) break;
    }

    // Clean up allocated memory for centroids
    for (Node* centroid : centroids) {
        delete centroid;
    }

    return clusters;
}

// Find approximate medoid based on clustering
int approximateMedoid(const vector<Node*>& nodes, int k) {
    if (nodes.empty()) {
        throw invalid_argument("Nodes are empty, cannot find medoid");
    }

    auto clusters = kMeansClustering(nodes, k);

    // Find the cluster with the most nodes
    int largestClusterIndex = 0;
    for (size_t i = 1; i < clusters.size(); ++i) {
        if (clusters[i].size() > clusters[largestClusterIndex].size()) {
            largestClusterIndex = i;
        }
    }

    if (clusters[largestClusterIndex].empty()) {
        throw runtime_error("Largest cluster is empty, cannot find medoid");
    }

    // Find centroid of the largest cluster
    Node* centroid = findCentroid(clusters[largestClusterIndex]);

    // Find the node within the largest cluster closest to the centroid
    int medoidIndex = -1;
    float minDist = numeric_limits<float>::max();
    for (const Node* node : clusters[largestClusterIndex]) {
        float dist = euclidean(node, centroid);
        if (dist < minDist) {
            minDist = dist;
            medoidIndex = node->id;
        }
    }

    delete centroid;
    return medoidIndex;
}
