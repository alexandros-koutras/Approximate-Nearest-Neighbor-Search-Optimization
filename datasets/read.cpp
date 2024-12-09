#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<int>> loadIvecs(const string& filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<vector<int>> data;

    while (true) {
        int numDims;
        infile.read(reinterpret_cast<char*>(&numDims), sizeof(int));
        if (infile.eof()) break;

        vector<int> vec(numDims);
        infile.read(reinterpret_cast<char*>(vec.data()), numDims * sizeof(int));

        data.push_back(vec);
    }

    infile.close();
    return data;
}

vector<vector<float>> loadFvecs(const string& filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<vector<float>> data;

    while (true) {
        int numDims; // First 4 bytes specify the number of dimensions
        infile.read(reinterpret_cast<char*>(&numDims), sizeof(int));
        if (infile.eof()) break;

        vector<float> vec(numDims); // Read the `float` vector
        infile.read(reinterpret_cast<char*>(vec.data()), numDims * sizeof(float));

        if (infile.gcount() != numDims * sizeof(float)) {
            cerr << "Error reading vector data. Incomplete vector found." << endl;
            break;
        }

        data.push_back(vec);
    }

    infile.close();
    return data;
}


void printIvecs(const vector<vector<int>>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        cout << "Vector " << i + 1 << ": ";
        for (int value : data[i]) {
            cout << value << " ";
        }
        cout << endl;
    }
}

void printFvecs(const vector<vector<float>>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        cout << "Vector " << i + 1 << ": ";
        for (float value : data[i]) {
            cout << value << " ";
        }
        cout << endl;
    }
}

int main() {
    // vector<vector<float>> fvecsData = loadFvecs("siftsmall_base.fvecs");
    // printFvecs(fvecsData);

    vector<vector<int>> ivecsData = loadIvecs("siftsmall_groundtruth.ivecs");
    printIvecs(ivecsData);

    return 0;
}
