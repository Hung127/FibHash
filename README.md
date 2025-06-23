# Fibonacci Hashing Project

## Introduction

Fibonacci hashing is a technique used to generate hash values by utilizing the Fibonacci sequence. This method leverages the golden ratio (approximately 1.618) to distribute keys uniformly across a hash table, reducing collisions and improving performance in hash-based data structures. The `FibHash.h` file contains the implementation of this hashing algorithm.

## Installation

1.  Ensure you have a C++ compiler installed (e.g., g++).
2.  Clone or download the repository containing the files: `.gitignore`, `BenchmarkAndAnalyst.h`, `FibHash.h`, `Record.h`, `main.cpp`, and this `README.md`.
3.  Navigate to the project directory in your terminal.
4.  Compile the project using the following command:
    
    ```bash
    g++ -o fibonacci_hash main.cpp
    
    ```
    
5.  The executable `fibonacci_hash` will be generated.

## Usage

1.  Run the compiled executable:
    
    ```bash
    ./fibonacci_hash
    
    ```
    
2.  The program will demonstrate the Fibonacci hashing algorithm. You can modify `main.cpp` to test different inputs or integrate the `FibHash.h` functions into your own projects.
3.  For benchmarking and analysis, include `BenchmarkAndAnalyst.h` and adjust the code as needed.

## Notes

-   Ensure all header files (`FibHash.h`, `BenchmarkAndAnalyst.h`, `Record.h`) are in the same directory as `main.cpp`.
-   Refer to the code comments in `FibHash.h` for detailed function explanations.
