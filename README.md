
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

## Code instruction

1.  **Compile the Code**:  
    Run the compilation command as shown above to generate an executable named `hello`.
2.  **Run with Parameters**:
    -   To see the help message:
        
        ```bash
        ./hello -h
        
        ```
        
    -   To process data with specific options (e.g., reading from `input.txt`, writing to `output.csv`, using 500 keys of `Random` type):
        
        ```bash
        ./hello -i input.txt -o output.csv 500 Random
        
        ```
        
    -   The program accepts the following command-line options:
        -   `-h`: Show this help message.
        -   `-i <file>`: Store data from the input file.
        -   `-o <file>`: Output results to the specified file.
        -   `<num>`: Input number of keys (integer).
        -   `<type>`: Input type of data (options: `Random`, `Sequential`, `Clustered`, `Fibonacci_Sensitive`, `Modulo_Sensitive`).
        -   **Example**:
            
            ```
            ./program -i test.txt -o test.csv 1000 Clustered
            
            ```
            
            -   Reads input from `test.txt`.
            -   Writes output to `test.csv`.
            -   Processes 1000 keys.
            -   Uses the `Clustered` data type.
3.  **Output**:
    -   By default, output is displayed in the console.
    -   If `-o <file>` is specified, results are saved to the designated `.csv` file instead.
4.  **Customize**:
    -   Modify `main.cpp` to adjust the logic or integrate with `FibHash.h` for Fibonacci hashing functionality.
    -   Ensure all input files (e.g., `test.txt`) exist and contain the expected data format.

## Notes

-   Ensure all header files (`FibHash.h`, `BenchmarkAndAnalyst.h`, `Record.h`) are in the same directory as `main.cpp`.
-   Refer to the code comments in `FibHash.h` for detailed function explanations.
-   The program expects the input file (if `-i` is used) to be in a format it can parse.
-   The output `.csv` file will contain the processed results, likely related to hashing performance or data distribution.
