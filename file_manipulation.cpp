#include "vamana.h"

// Function to read .ivecs file
vector<Node*> load_ivecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            int value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(int))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << value << endl;
        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}

// Function to read .fvecs file
vector<Node*> load_fvecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        //  Read dimensions
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;  
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            //  Read each coordinate
            float value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(float))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << value << endl;

        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}

// Function to read .bvecs file
vector<Node*> load_bvecs(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Node*> nodes;
    while (true) {
        int d;
        if (!input.read(reinterpret_cast<char*>(&d), sizeof(int))) {
            if (input.eof()) break;
            cerr << "Error reading dimensions from file." << endl;
            break;
        }
        
        Node* node = new Node();
        node->coords.resize(d);
        for (int i = 0; i < d; ++i) {
            unsigned char value;
            if (!input.read(reinterpret_cast<char*>(&value), sizeof(unsigned char))) {
                cerr << "Error reading coordinates from file." << endl;
                break;
            }
            node->coords[i] = value;
            cout << "Value " << i << ": " << static_cast<int>(value) << endl;
        }
        node->id = nodes.size();  // Assign node ID sequentially
        nodes.push_back(node);
    }
    input.close();

    return nodes;
}