#ifndef FIB_HASH_H
#define FIB_HASH_H
#include <iostream>
#include <vector>
#include <functional>
#define uint unsigned int
#define FIB_CONST 2654435759
#define WORD_SIZE 32

uint getLog2(uint num) {
    // every number needs at least 1 bit to represent
    uint count = 1;
    while ((num >> count) > 1) {
        count++;
    }
    return count;
}

class HashTable {
private:
    struct Entry {
        uint key;
        bool isDeleted;
        bool isOccupied;
        // constructor for entries
        Entry() : key(0), isDeleted(false), isOccupied(false) {}
    };
    
    // size of the hash table
    uint size;
    // sizeLog = log2(size )
    uint sizeLog;
    // number of keys
    uint count;
    // hash table
    std::vector<Entry> table;

    // function pointer for hashFunction choice (something like that)
    std::function<uint(uint)> hashFunction;

    uint fibHashFunction(uint key) {
        // use ((unsigned long long) key * FIB_CONST) & ((1ull << WORD_SIZE) - 1) to make sure 
        // that it will always fits int 32 unsigned bits (number <= 2^32 - 1)
        return (((unsigned long long) key * FIB_CONST) & ((1ull << WORD_SIZE) - 1)) >> (WORD_SIZE - sizeLog);
    }

    uint moduloHashFunction(uint key) {
        return (key % size);
    }

    void resize(uint newSize, int newSizeLog) {
        // create a new vector to hold old keys
        std::vector<Entry> oldTable(table);
        size = newSize;
        sizeLog = newSizeLog;
        // create a new table
        table = std::vector<Entry>(newSize);
        count = 0;
        for (const Entry &e : oldTable) {
            // empty slot or deleted slot
            if (e.isDeleted || !e.isOccupied) {
                continue;
            }
            if (!insert(e.key)) {
                std::cout << "FAULT with key " << e.key << "\n";
                return;
            }
        }
    }

public:
    bool insert(uint key) {
        if (2u * count >= size) {
            resize(size * 2, sizeLog + 1);
        }

        bool foundFirstDeleted = false;
        uint firstDeletedIdx = 0; // set a temp value for firstDeleted idx
        uint i = 0;
        uint hash = hashFunction(key);
        // linear probing
        while (i < size) {
            uint idx = (hash + i) & (size - 1);
            // found empty slot
            if (!table[idx].isOccupied) {
                break;
                // deleted
            } else if (table[idx].isDeleted) {
                // store the first deleted index
                if (!foundFirstDeleted) {
                    foundFirstDeleted = true;
                    firstDeletedIdx = idx;
                }
                // already existed
            } else if (table[idx].key == key) {
                return false;
            }
            i++;
        }
        // always make sure that if there is any deleted slot, insert to that first, then empty slot
        uint insertIdx = foundFirstDeleted ? firstDeletedIdx : (hash + i) & (size - 1);
        // insertion
        table[insertIdx].key = key;
        table[insertIdx].isOccupied = true;
        table[insertIdx].isDeleted = false;
        count++;
        return true;
    }

    bool remove(uint key) {
        uint hash = hashFunction(key);
        // linear probing
        uint i = 0;
        while (i < size)  {
            uint idx = (hash + i) & (size - 1);
            // empty slot
            if (!table[idx].isOccupied) {
                return false;
                // key found
            } else if (table[idx].key == key) {
                // key found and not deleted
                if (!table[idx].isDeleted) {
                    table[idx].isDeleted = true;
                    count--;
                    return true;
                }
                // key found but deleted
                return false;
            } 
            i++;
        }
        // do not found any key
        return false;
    }

    bool search(uint key) {
        uint hash = hashFunction(key);
        // linear probing
        uint i = 0;
        while (i < size)  {
            uint idx = (hash + i) & (size - 1);
            // empty slot
            if (!table[idx].isOccupied) {
                return false;
                // key found
            } else if (table[idx].key == key) {
                // key found and not deleted
                if (!table[idx].isDeleted) {
                    return true;
                }
                // key found but deleted
                return false;
            }
            i++;
        }
        // do not found any key
        return false;
    }

    // assume that tableSize is always be a power of 2
    HashTable(uint tableSize, bool useFibonacciHashing = true) : size(std::max(1u, tableSize)),
                        sizeLog(getLog2(tableSize)), table(std::max(1u, tableSize)), count(0) {
        // choice of use hash function
        if (useFibonacciHashing) {
            hashFunction = [this](uint key) { return this->fibHashFunction(key); };
        } else {
            hashFunction = [this](uint key) { return this->moduloHashFunction(key); };
        }
    }
};

#endif