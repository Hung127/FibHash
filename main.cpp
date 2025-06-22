#include "FibHash.h"
#include "Record.h"
#include "BenchmarkAndAnalyst.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

// Assuming dataTypes is defined somewhere - if not, you'll need to define it
extern std::vector<std::string> dataTypes; // or define it here if needed

bool isNumber(const std::string& s) {
    if (s.empty()) return false; // Handle empty string
    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

void showHelp() {
    std::cout << "Usage: ./program [OPTIONS]\n"
        << "Options:\n"
        << " -h                Show this help message\n"
        << " -i <file>         Store data of file input\n"
        << " -o <file>         Output results to file\n"
        << " <num>             Input number of keys (integer)\n"
        << " <type>            Input type of data (Random, Sequential, Clustered, Fibonacci_Sensitive, Modulo_Sensitive)\n"
        << "Example:\n"
        << " ./program -i test.txt -o test.csv 1000 Clustered\n";
}

void parseArgs(int argc, char* argv[], std::string& fileIn, std::string& fileOut, int& numKeys, std::string& type) {
    // Maybe that users do not know how to use this program, so they pass nothing to command line argument
    if (argc <= 1) {
        showHelp();
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            showHelp();
            exit(0);
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            i++; // Move to next argument
            fileIn = argv[i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            i++; // Move to next argument
            fileOut = argv[i];
        }
        else if (isNumber(argv[i])) {
            numKeys = std::stoi(argv[i]);
        }
        else if (std::find(dataTypes.begin(), dataTypes.end(), argv[i]) != dataTypes.end()) {
            type = argv[i];
        }
    }
}

void processArg(int argc, char* argv[]) {
    std::string fileIn, fileOut, type;
    int numKeys = 0;
    parseArgs(argc, argv, fileIn, fileOut, numKeys, type);
    runExperiment(fileIn, numKeys, type, fileOut);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        showHelp();
        return 0;
    }
    processArg(argc, argv);
    return 0;
}