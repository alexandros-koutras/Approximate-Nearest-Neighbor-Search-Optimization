# Approximate-Nearest-Neighbor-Search-Optimization


---


## 💡 Overview

This project is an implementation of advanced graph-based algorithms for Approximate Nearest Neighbor Search (ANNS), specifically focusing on Filtered ANNS. Developed in C++, the application efficiently finds the k nearest neighbors of a query vector, given an associated filter/label.


---


## ⚙️ Features

- **Filtered ANNS (F-Vamana)**: Implementation of the Filtered Vamana indexing algorithm and the associated Filtered Greedy Search for efficient queries with label filters.
- **Stitched ANNS (S-Vamana)**: Implementation of the Stitched Vamana algorithm, which constructs the graph by merging (stitching) smaller Vamana indices built on labeled subsets of the data.
- **Graph Pruning**: Includes the Filtered Robust Prune procedure to maintain a bounded maximum out-degree for the graph.
- **Performance Optimization**: Incorporates performance enhancements achieved through:
- Parallel Execution using threading (e.g., pthreads or std::thread/OpenMP).
- Optimizations such as calculating distances, initializing graphs with random edges, and modifying medoid selection.
- **Software Testing**: Utilizes appropriate libraries for software testing and verifies correctness automatically via GitHub Actions upon every commit/push.


---


## 🧰 Technologies Used

- **Language**: C/C++
- **Version Control**: Git (mandatory)
- **Automation**: GitHub Actions for automated testing and correctness checks
- **Makefile**: Build automation


---


## 🧑‍💻 How to Build and Run

The provided Makefile includes preset targets for easy execution of the various phases of the assignment.

### 1. Clone the repository
git clone https://github.com/alexandros-koutras/Approximate-Nearest-Neighbor-Search-Optimization
cd Approximate-Nearest-Neighbor-Search-Optimization

### 2. Compile and run
You can execute the program directly using the predefined Makefile targets:

make run

This command runs the program with the default set of arguments defined in the Makefile.

If you wish to test the program with alternative argument configurations, you can specify a numbered variant (from 1 to 4) as follows:

make run1
make run2
make run3
make run4

Each target corresponds to a different predefined argument set, allowing quick testing under various parameter combinations.


---


## 📂 Project Structure

Approximate-Nearest-Neighbor-Search-Optimization/
├── bruteforce/             # Implemantation of bruteforce and tests
├── include/                # Header files (.h) for all functions, structs, and interfaces
├── modules/                # Source files (.cpp) containing the core logic
├── tests/                  # Directory for unit and integration tests (using the selected library)
├── datasets/               # Input datasets (following SIGMOD 2024 specifications)
├── main.cpp                # The Main script to execute the code
├── Makefile                # Build automation for compilation and testing
├── Project proposal.docx   # A doc where we analyse all the optimizations we did
└── README.md               # Project documentation


---


## 👥 Authors

The project was done by a group of 3 people

Name                        Contribution
---
Alexandros Koutras(me)      Stitched Vamana Implementation, Filtered Robust Prune, Parallel (Threads) Stitched Vamana, Parallel
                            (Threads) Greedy Search in Main, Save and Reading the Graph, Main Function Logic, Brute Force,
                            Groundtruth calculation

Anastasia Orfanoudaki       Find_Medoid with Filters, Filtered Greedy Search, Threads Optimization for Search and
                            Distance Calculations using Threads, Project proposal creation

Ioulia Osadtsiv             Filtered Vamana Implementation and its Optimizations, Stitched Vamana Optimizations, Project proposal
                            creation
