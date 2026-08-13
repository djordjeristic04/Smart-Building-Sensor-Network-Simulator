# Smart Building Sensor Network Simulator

## Project Description

This repository contains a C++ implementation of a sensor network simulator, developed as a coursework project for the Algorithms and Data Structures course at the School of Electrical Engineering, University of Belgrade (academic year 2025/26).

The system models a network of sensors in a smart building as a directed, weighted graph: nodes represent sensors (each with a unique ID and a processing speed), and edges represent communication links, weighted by the signal travel time between sensors. The existence and weight of each edge are determined pseudo-randomly, based on a probability threshold and randomly generated sensor distances.

As required by the assignment, the project uses **no STL containers** and **no recursion** — all data structures (including the graph representation, queue, and stack used for traversal) are implemented from scratch.

## Key Features

- **Custom Pseudo-Random Number Generator:** Implements the Blum Blum Shub (BBS) algorithm with parity-bit extraction to generate pseudo-random integers, reals in `[0, 1)`, and integers within a given range.
- **Graph Construction:** Builds a directed weighted graph using an adjacency matrix, where edge existence is determined probabilistically and edge weights are computed from randomly generated sensor distances and processing speeds.
- **Dynamic Graph Editing:** Supports adding and removing sensors at runtime, with automatic resizing of the underlying adjacency matrix and sensor array.
- **Graph Traversal:** Iterative (non-recursive) BFS and DFS implementations using custom `Queue` and `Stack` data structures, visiting neighbors in increasing ID order.
- **Shortest Path:** Dijkstra's algorithm to compute the shortest signal travel time and path between two sensors.
- **Graph Analysis:** Computes the eccentricity of every sensor and identifies the central sensor of the network.
- **Interactive Menu:** A console-based menu allows the user to perform all operations in any order, with input available either from the keyboard or from a text file.

## Project Structure

- `main.cpp` — program entry point and interactive menu loop.
- `Generator.h / .cpp` — pseudo-random number generator (BBS algorithm with parity bit).
- `Graph.h / .cpp` — graph representation (adjacency matrix), sensor management, traversal, and analysis algorithms; also contains the custom `Queue` and `Stack` structures used for BFS/DFS.

## How to Run

1. Clone the repository.
2. Compile the source files with a C++ compiler, for example:
   ```
   g++ -std=c++17 main.cpp Generator.cpp Graph.cpp -o sensor_network
   ```
3. Run the compiled executable. Choose whether to provide input via the keyboard or a text file, enter a seed for the random number generator, and use the on-screen menu to generate sensors, build the graph, and run the available operations (BFS, DFS, shortest path, eccentricity, etc.).

## Note

This project was developed as an individual coursework assignment. Since the assignment specification may be reused in future course iterations, please do not copy this code for your own submission if you are a student currently working on the same assignment.

## Author

Đorđe Ristić — Undergraduate student, School of Electrical Engineering, University of Belgrade.
