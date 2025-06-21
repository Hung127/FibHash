#ifndef	BenchmarkAnalyst_h
#define BenchmarkAnalyst_h

#pragma once
#include "FibHash.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <iomanip>
#include <fstream>
#include "Record.h"

#define MAX_KEY (1u << 30)
#define TABLE_SIZE 4096u // 2^12



std::vector<uint> generateKeys(int count, const std::string& type) {
	std::vector<uint> keys;
	std::random_device rd; // Initialize seed for std::mt19937
	std::mt19937 gen(rd()); // Random number generator

	if (type == "Random") {
		std::uniform_int_distribution<> dis(0, MAX_KEY); // Random distribution
		for (int i = 0; i < count; i++) {	
			keys.push_back(dis(gen));
		}
	}
	else if (type == "Sequential") {
		for (int i = 0; i < count; i++) {
			keys.push_back(i);
		}
	}
	else if (type == "Clustered") {
		uint randomNum = gen() % ((MAX_KEY) - 100);
		const uint RANDOM_RANGE = 100;
		std::uniform_int_distribution<> dis(randomNum, randomNum + RANDOM_RANGE);
		for (int i = 0; i < count; i++) {
			keys.push_back(dis(gen));
		}
	} else if (type == "Fibonacci Sensitive") {
		// generate a fibonacci sequence
		keys.push_back(0);
		keys.push_back(1);
		for (int i = 2; i < count; i++) {
			// % MAX_KEY to ensure that it does not exceed the max
			keys.push_back((keys[i - 1] + keys[i - 2]) % MAX_KEY);
		}
		// shuffle the array
		std::shuffle(keys.begin(), keys.end(), gen);
	} else if (type == "Modulo Sensitive") {
		for (uint i = 0; i < count; i++) {
			// % MAX_KEY to ensure that it does not exceed the max
			keys.push_back((TABLE_SIZE * i) % (MAX_KEY));
		}
	}
	return keys;
}

void generateTest(const std::string& filename, int numKeys, const std::string& keyType) {
	std::ofstream out(filename);

	if (!out.is_open()) {
		std::cerr << "File error\n";
		return;
	}

	std::cout << "Generating test for " << filename << " with " << numKeys << " keys of type " << keyType << std::endl;
	// first line is the array length
	out << numKeys << "\n";
	std::vector<uint> arr = generateKeys(numKeys, keyType);
	for (int i = 0; i < numKeys; i++) {
		out << arr[i] << (i < numKeys - 1 ? " " : "\n");
	}

	out.close();
	std::cout << "Generated test for " << filename << " with " << numKeys << " keys of type " << keyType << std::endl;
}
	

// input file is the file that we logged the array into it (input file should be a blank file)
// output file is the file that stores result
void runExperiment(const std::string& inputFile, int numKeys, const std::string& keyType, const std::string &outputFile) {
	std::vector<uint> keys = generateKeys(numKeys, keyType);
	std::cout << "Experiment with " << numKeys << " keys (" << keyType << ")\n";

	generateTest(inputFile, numKeys, keyType);
	// Fibonacci Hashing
	HashTable htFib(TABLE_SIZE, true);
	
	// Measure time for insert
	auto start = std::chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htFib.insert(key);
	}
	auto end = std::chrono::high_resolution_clock::now();
	double insertTime = std::chrono::duration<double, std::micro>(end - start).count();

	// Measure time for search
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htFib.search(key);
	}
	end = std::chrono::high_resolution_clock::now();
	double searchTime = std::chrono::duration<double, std::micro>(end - start).count();

	// Measure time for remove
	start = std::chrono::high_resolution_clock::now();
	for (int key : keys) htFib.remove(key);
	end = std::chrono::high_resolution_clock::now();
	double removeTime = std::chrono::duration<double, std::micro>(end - start).count();

	std::ofstream os(outputFile);

	os << std::fixed << std::setprecision(2);
	std::cout << std::fixed << std::setprecision(2);

	std::vector<std::string> statLabels = {
		"Time for insert: ",
		"Time for search: ",
		"Time for remove: ",
		"Collision rate: ",
		"Average Probe Length (insert): ",
		"Average Probe Length (search): ",
		"Average Probe Length (remove): ",
		"Maximum Probe Length (insert): ",
		"Maximum Probe Length (search): ",
		"Maximum Probe Length (remove): "
	};
	
	std::vector<std::string> statValues = {
		std::to_string(insertTime) + " µs",
		std::to_string(searchTime) + " µs",
		std::to_string(removeTime) + " µs",
		std::to_string(htFib.getCollisionRate() * 100) + "%",
		std::to_string(htFib.getAVGInsertionProbing()),
		std::to_string(htFib.getAVGSearchProbing()),
		std::to_string(htFib.getAVGRemoveProbing()),
		std::to_string(htFib.getMaxInsertProbing()),
		std::to_string(htFib.getMaxSearchProbing()),
		std::to_string(htFib.getMaxRemoveProbing())
	};

	if (os.is_open()) {
		Record rec("FibonacciHashing " + keyType);
		Record title(statLabels);
		Record stats(statValues);

		rec.writeToFile(os);
		title.writeToFile(os);
		stats.writeToFile(os);
	}

	std::cout << "Fibonacci Hashing\n";
	for (int i = 0; i < statLabels.size(); ++i) {
		std::cout << " " << statLabels[i] << statValues[i] << '\n';
	}
	
	// ------------------------------------------------------------------------------------------------

	// Modulo Hashing
	HashTable htMod(TABLE_SIZE, false);

	// Measure time for insert
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htMod.insert(key);
	}
	end = std::chrono::high_resolution_clock::now();
	insertTime = std::chrono::duration<double, std::micro>(end - start).count();

	// Measure time for search
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htMod.search(key);
	}
	end = std::chrono::high_resolution_clock::now();
	searchTime = std::chrono::duration<double, std::micro>(end - start).count();

	// Measure time for remove
	start = std::chrono::high_resolution_clock::now();
	for (int key : keys) htMod.remove(key);
	end = std::chrono::high_resolution_clock::now();
	removeTime = std::chrono::duration<double, std::micro>(end - start).count();

	std::vector<std::string> statValues2 = {
		std::to_string(insertTime) + " µs",
		std::to_string(searchTime) + " µs",
		std::to_string(removeTime) + " µs",
		std::to_string(htMod.getCollisionRate() * 100) + "%",
		std::to_string(htMod.getAVGInsertionProbing()),
		std::to_string(htMod.getAVGSearchProbing()),
		std::to_string(htMod.getAVGRemoveProbing()),
		std::to_string(htMod.getMaxInsertProbing()),
		std::to_string(htMod.getMaxSearchProbing()),
		std::to_string(htMod.getMaxRemoveProbing())
	};

	if (os.is_open()) {

		Record title2(statLabels);
		Record stats2(statValues2);
		Record rec2("Modulo Hashing " + keyType);

		rec2.writeToFile(os);
		title2.writeToFile(os);
		stats2.writeToFile(os);
		os.close();
	}

	std::cout << "Modulo Hashing\n";
	for (int i = 0; i < statLabels.size(); ++i) {
		std::cout << " " << statLabels[i] << statValues2[i] << '\n';
	}

}
#endif // !BenchmarkAnalyst_h
