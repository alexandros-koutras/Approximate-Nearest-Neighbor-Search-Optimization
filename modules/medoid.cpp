#include "../include/vamana.h"

//find the centroid of a cluster
Node* findCentroid(const vector<Node*>& cluster) {
    size_t dimensions = cluster[0]->coords.size();//number of dimensions
    Node* centroid = new Node;//centroid node and coordinates initialization
    centroid->coords.resize(dimensions, 0.0);

    //Sum up the coordinates of each node
    for (const Node* point : cluster) {
        for (size_t i = 0; i < dimensions; ++i) {
            centroid->coords[i] += point->coords[i];
        }
    }
    //Divide each coordinate sum by the number of points to get the centroid
    for (double& value : centroid->coords) {
        value /= cluster.size();
    }
    return centroid;
}

// K-means clustering
vector<vector<Node*>> kMeansClustering(const vector<Node*>& nodes, int k, int maxIterations = 100) {
    vector<Node*> centroids;
    vector<vector<Node*>> clusters(k);//vector of clusters, one for each centroid

    // Initialize random centroids
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(nodes[rand() % nodes.size()]);
    }
    //K-means iterations
    for (int iter = 0; iter < maxIterations; ++iter) {
        // Assign each node to the nearest centroid
        for (const Node* node : nodes) {
            int closestCluster = 0;
            double minDist = euclidean(node, centroids[0]);//dist to the first centroid
            for (int j = 1; j < k; ++j) {
                double dist = euclidean(node, centroids[j]);
                if (dist < minDist) {//if a nearer centroid is found 
                    minDist = dist;//update closest cluster 
                    closestCluster = j;
                }
            }
            clusters[closestCluster].push_back(const_cast<Node*>(node));
        }

        // Recalculate centroids
        bool centroidsChanged = false;
        for (int i = 0; i < k; ++i) {
            Node* newCentroid = findCentroid(clusters[i]);
            if (euclidean(newCentroid, centroids[i]) > 1e-4) {// Check if the centroid has changed significantly, 1e-4 is a small threshold
                centroidsChanged = true;
            }
            centroids[i] = newCentroid;
        }

        // If centroids don't change, break early
        if (!centroidsChanged) break;

        // Clear clusters for the next iteration (except the last one)
        if (iter < maxIterations - 1) {
            for (auto& cluster : clusters) {
                cluster.clear();
            }
        }
    }

    return clusters;
}

//Find approximate medoid based on clustering
int approximateMedoid(const vector<Node*>& nodes, int k) {
    auto clusters = kMeansClustering(nodes, k);

    // Find the cluster with the most nodes
    int largestClusterIndex = 0;
    for (unsigned int i = 1; i < clusters.size(); ++i) {
        if (clusters[i].size() > clusters[largestClusterIndex].size()) {
            largestClusterIndex = i;
        }
    }

    //Find centroid of the largest cluster
    Node* centroid = findCentroid(clusters[largestClusterIndex]);

     //Find the node within the largest cluster closest to the centroid
     int medoidIndex = -1;
    double minDist = numeric_limits<double>::max();
    for (const Node* node : clusters[largestClusterIndex]) {
        double dist = euclidean(node, centroid);
        if (dist < minDist) {
            minDist = dist;
            medoidIndex = node->id;
        }
    }

    delete centroid; 
    return medoidIndex; 
}


