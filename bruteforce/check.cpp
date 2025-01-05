#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cassert>

using namespace std;


struct Node {
    unsigned int id;
    float distance;
    vector<float> coords;
    vector<Node*> out_neighbors; 
    float filter;
};

vector<vector<float>> ReadGroundTruth(const string& file_path) {
    cout << "Reading Ground Truth: " << file_path << endl;
    ifstream ifs(file_path, ios::binary);
    assert(ifs.is_open());

    uint32_t N;
    ifs.read(reinterpret_cast<char*>(&N), sizeof(uint32_t));  // Number of points
    size_t num_dimensions;
    ifs.read(reinterpret_cast<char*>(&num_dimensions), sizeof(size_t));  // Number of dimensions

    vector<vector<float>> data(N, vector<float>(num_dimensions));
    for (auto& row : data) {
        ifs.read(reinterpret_cast<char*>(row.data()), num_dimensions * sizeof(float));
    }

    ifs.close();
    return data;
}

void print_vectors(vector<float> coords) {
    for (float c : coords) {
        cout << c << " ";
    }
}

void print_groundtruth(vector<vector<float>> groundtruth) {
    for (size_t i = 0; i < groundtruth.size(); i++) {
        cout << "Node id " << i << endl;
        print_vectors(groundtruth.at(i));
        cout << endl << endl;
    }
}

int main() {
    vector<vector<float>> data = ReadGroundTruth("dummy-groundtruth.bin");

    print_groundtruth(data);

    cout << endl;
}