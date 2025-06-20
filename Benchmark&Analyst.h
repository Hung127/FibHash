#ifndef Benchmark&Analyst_h
#define Benchmark&Analyst_h

#include "FibHash.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

vector<uint> generateKeys(int count, const string& type) {
	vector<uint> keys;
	random_device rd; // Initialize seed for mt19937
	mt19937 gen(rd()); // Random number generator

	if (type == "Random") {
		uniform_int_distribution<> dis(0, MAX_KEY); // Random distribution
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
		uniform_int_distribution<> dis(5000, 5100);
		for (int i = 0; i < count; i++) {
			keys.push_back(dis(gen));
		}
	}

	return keys;
}

void generateTest(const string& filename, int numKeys, const string& keyType) {
	ofstream out(filename);

	if (!out.is_open()) {
		std::cerr << "File error\n";
		return;
	}

	out << numKeys << "\n";
	vector<uint> arr = generateKeys(numKeys, keyType);
	for (int i = 0; i < numKeys; i++) {
		out << arr[i] << (i < numKeys - 1 ? " " : "\n");
	}

	out.close();
	cout << "Generating test for " << filename << " with " << numKeys << " keys of type " << keyType << endl;
}
	

void runExperiment(const string& filename, int numKeys, const string& keyType) {
	vector<uint> keys = generateKeys(numKeys, keyType);
	cout << "\Experiment with " << numKeys << " keys (" << keyType << ")\n";

	generateTest(filename, numKeys, keyType);
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

	cout << std::fixed << std::setprecision(2);
	cout << "Fibonacci Hashing:\n";
	cout << " Time for insert: " << insertTime << " 탎\n";
	cout << " Time for search: " << searchTime << " 탎\n";
	cout << " Ratio Collision: " << htFib.getCollisionRate() * 100 << "%\n";
	cout << " Average Probe Length (insert): " << htFib.getAVGInsertionProbing() << "\n";
	cout << " Average Probe Length (search): " << htFib.getAVGSearchProbing() << "\n";
	cout << " Average Probe Length (remove): " << htFib.getAVGRemoveProbing() << "\n";
	cout << " Maximum Probe Length (insert): " << htFib.getMaxInsertProbing() << "\n";
	cout << " Maximum Probe Length (search): " << htFib.getMaxSearchProbing() << "\n";
	cout << " Maximum Probe Length (remove): " << htFib.getMaxRemoveProbing() << "\n";
	
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

	cout << "Modulo Hashing:\n";
	cout << " Time for insert: " << insertTime << " 탎\n";
	cout << " Time for search: " << searchTime << " 탎\n";
	cout << " Ratio Collision: " << htMod.getCollisionRate() * 100 << "%\n";
	cout << " Average Probe Length (insert): " << htMod.getAVGInsertionProbing() << "\n";
	cout << " Average Probe Length (search): " << htMod.getAVGSearchProbing() << "\n";
	cout << " Average Probe Length (search): " << htMod.getAVGRemoveProbing() << "\n";
	cout << " Maximum Probe Length (insert): " << htMod.getMaxInsertProbing() << "\n";
	cout << " Maximum Probe Length (search): " << htMod.getMaxSearchProbing() << "\n";
	cout << " Maximum Probe Length (remove): " << htMod.getMaxRemoveProbing() << "\n";
}
#endif // !Benchmark&Analyst_h
