#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

using namespace std;


vector<vector<float>> ReadGroundTruth(const string& file_path) {
    cout << "Reading Ground Truth: " << file_path << endl;
    ifstream ifs(file_path, ios::binary);
    if (!ifs.is_open()) {
        cerr << "Error: Failed to open file " << file_path << endl;
        return {};  // Return empty data if file can't be opened
    }

    uint32_t N;
    ifs.read(reinterpret_cast<char*>(&N), sizeof(uint32_t));  // Number of points
    cout << "Number of points (N): " << N << endl;

    if (ifs.fail()) {
        cerr << "Error: Failed to read the number of points" << endl;
        return {};  // Return empty data if reading fails
    }

    vector<vector<float>> data(N);

    // Read each row with a different number of dimensions
    for (uint32_t i = 0; i < N; ++i) {
        size_t num_dimensions;
        ifs.read(reinterpret_cast<char*>(&num_dimensions), sizeof(size_t));  // Number of dimensions for this row
        cout << "Row " << i << " - Number of dimensions: " << num_dimensions << endl;

        if (ifs.fail()) {
            cerr << "Error: Failed to read the number of dimensions for row " << i << endl;
            break;
        }

        vector<float> row(num_dimensions);
        ifs.read(reinterpret_cast<char*>(row.data()), num_dimensions * sizeof(float));  // Read the row data

        if (ifs.fail()) {
            cerr << "Error: Failed to read the data for row " << i << endl;
            break;
        }

        // Print the raw data for verification
        cout << "Row " << i << " data: ";
        for (size_t j = 0; j < row.size(); ++j) {
            cout << row[j] << " ";
        }
        cout << endl;

        data[i] = move(row);  // Store the row in the data vector
    }

    ifs.close();
    cout << "Finished Reading Ground Truth" << endl;
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
    vector<vector<float>> groundtruth = ReadGroundTruth("dummy-groundtruth.bin");
    print_groundtruth(groundtruth);
}