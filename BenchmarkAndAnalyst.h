#ifndef	BenchmarkAnalyst_h
#define BenchmarkAnalyst_h

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
	std::srand((unsigned)std::time(NULL));
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
		uint randomNum = rand() % ((MAX_KEY) - 100);
		const uint RANDOM_RANGE = 100;
		uniform_int_distribution<> dis(randomNum, randomNum + RANDOM_RANGE);
		for (int i = 0; i < count; i++) {
			keys.push_back(dis(gen));
		}
	} else if (type == "Fibonacci Sensitive") {
		// generate a fibonacci sequence
		keys.push_back(0);
		keys.push_back(1);
		for (int i = 2; i < count; i++) {
			// % MAX_KEY to ensure that it does not exceed the max
			keys.push_back(keys[i - 1] + keys[i - 2] % (MAX_KEY));
		}
		// shuffle the array
		for (int i = keys.size() - 1; i >= 0; i--) {
      
			// Generate the random index 
			int j = rand() % (i + 1);
			swap(keys[i], keys[j]);
    	}

	} else if (type == "Modulo Sensitive") {
		for (uint i = 0; i < count; i++) {
			// % MAX_KEY to ensure that it does not exceed the max
			keys.push_back((TABLE_SIZE * i) % (MAX_KEY));
		}
	}
	return keys;
}

void generateTest(const string& filename, int numKeys, const string& keyType) {
	ofstream out(filename);

	if (!out.is_open()) {
		cerr << "File error\n";
		return;
	}

	cout << "Generating test for " << filename << " with " << numKeys << " keys of type " << keyType << endl;
	// first line is the array length
	out << numKeys << "\n";
	vector<uint> arr = generateKeys(numKeys, keyType);
	for (int i = 0; i < numKeys; i++) {
		out << arr[i] << (i < numKeys - 1 ? " " : "\n");
	}

	out.close();
	cout << "Generated test for " << filename << " with " << numKeys << " keys of type " << keyType << endl;
}
	

// input file is the file that we logged the array into it (input file should be a blank file)
// output file is the file that stores result
void runExperiment(const string& inputFile, int numKeys, const string& keyType, const string &outputFile) {
	vector<uint> keys = generateKeys(numKeys, keyType);
	cout << "Experiment with " << numKeys << " keys (" << keyType << ")\n";

	generateTest(inputFile, numKeys, keyType);
	// Fibonacci Hashing
	HashTable htFib(TABLE_SIZE, true);
	
	// Measure time for insert
	auto start = chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htFib.insert(key);
	}
	auto end = chrono::high_resolution_clock::now();
	double insertTime = chrono::duration<double, micro>(end - start).count();

	// Measure time for search
	start = chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htFib.search(key);
	}
	end = chrono::high_resolution_clock::now();
	double searchTime = chrono::duration<double, micro>(end - start).count();

	// Measure time for remove
	start = chrono::high_resolution_clock::now();
	for (int key : keys) htFib.remove(key);
	end = chrono::high_resolution_clock::now();
	double removeTime = chrono::duration<double, micro>(end - start).count();

	ofstream os(outputFile);

	os << fixed << setprecision(2);
	cout << fixed << setprecision(2);

	vector<string> statLabels = {
		"Time for insert: ",
		"Time for search: ",
		"Collision rate: ",
		"Average Probe Length (insert): ",
		"Average Probe Length (search): ",
		"Average Probe Length (remove): ",
		"Maximum Probe Length (insert): ",
		"Maximum Probe Length (search): ",
		"Maximum Probe Length (remove): "
	};
	
	vector<string> statValues = {
		to_string(insertTime) + " µs",
		to_string(searchTime) + " µs",
		to_string(htFib.getCollisionRate() * 100) + "%",
		to_string(htFib.getAVGInsertionProbing()),
		to_string(htFib.getAVGSearchProbing()),
		to_string(htFib.getAVGRemoveProbing()),
		to_string(htFib.getMaxInsertProbing()),
		to_string(htFib.getMaxSearchProbing()),
		to_string(htFib.getMaxRemoveProbing())
	};

	if (os.is_open()) {
		Record rec("FibonacciHashing " + keyType);
		Record title(statLabels);
		Record stats(statValues);

		rec.writeToFile(os);
		title.writeToFile(os);
		stats.writeToFile(os);
	}

	cout << "Fibonacci Hashing\n";
	for (int i = 0; i < statLabels.size(); ++i) {
		cout << " " << statLabels[i] << statValues[i] << '\n';
	}
	
	// ------------------------------------------------------------------------------------------------

	// Modulo Hashing
	HashTable htMod(TABLE_SIZE, false);

	// Measure time for insert
	start = chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htMod.insert(key);
	}
	end = chrono::high_resolution_clock::now();
	insertTime = chrono::duration<double, micro>(end - start).count();

	// Measure time for search
	start = chrono::high_resolution_clock::now();
	for (unsigned int key : keys) {
		htMod.search(key);
	}
	end = chrono::high_resolution_clock::now();
	searchTime = chrono::duration<double, micro>(end - start).count();

	// Measure time for remove
	start = chrono::high_resolution_clock::now();
	for (int key : keys) htMod.remove(key);
	end = chrono::high_resolution_clock::now();
	removeTime = chrono::duration<double, micro>(end - start).count();


	vector<string> statLabels2 = {
		"Time for insert: ",
		"Time for search: ",
		"Collision rate: ",
		"Average Probe Length (insert): ",
		"Average Probe Length (search): ",
		"Average Probe Length (remove): ",
		"Maximum Probe Length (insert): ",
		"Maximum Probe Length (search): ",
		"Maximum Probe Length (remove): "
	};

	vector<string> statValues2 = {
		to_string(insertTime) + " µs",
		to_string(searchTime) + " µs",
		to_string(htMod.getCollisionRate() * 100) + "%",
		to_string(htMod.getAVGInsertionProbing()),
		to_string(htMod.getAVGSearchProbing()),
		to_string(htMod.getAVGRemoveProbing()),
		to_string(htMod.getMaxInsertProbing()),
		to_string(htMod.getMaxSearchProbing()),
		to_string(htMod.getMaxRemoveProbing())
	};

	if (os.is_open()) {

		Record title2(statLabels2);
		Record stats2(statValues2);
		Record rec2("Modulo Hashing " + keyType);

		rec2.writeToFile(os);
		title2.writeToFile(os);
		stats2.writeToFile(os);
		os.close();
	}

	cout << "Modulo Hashing\n";
	for (int i = 0; i < statLabels2.size(); ++i) {
		cout << " " << statLabels2[i] << statValues2[i] << '\n';
	}

}
#endif // !BenchmarkAnalyst_h
