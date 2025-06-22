#ifndef FibHash_h
#define FibHash_h

#include <iostream>
#include <vector>
#include <functional>

using uint = unsigned int;
constexpr unsigned int FIB_CONST = 2654435759u;
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

    struct TestSpecs {
        unsigned long long totalSearchProbing;
        unsigned long long totalInsertProbing;
        unsigned long long totalDeleteProbing;
        uint maxInsertProbing;
        uint maxSearchProbing;
        uint maxDeleteProbing;
        uint insertCount;
        uint searchCount;
        uint deleteCount;
        uint insertionCollisions;
        TestSpecs() : totalDeleteProbing(0), totalInsertProbing(0),
                        totalSearchProbing(0), maxInsertProbing(0),
                        maxSearchProbing(0), maxDeleteProbing(0),
                        insertCount(0), searchCount(0), deleteCount(0), insertionCollisions(0) {}
    };

    TestSpecs status;
    
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
        TestSpecs newStatus;
        status = newStatus;
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
        // if (2u * count >= size) {
        //     resize(size * 2, sizeLog + 1);
        // }
        // udate status
        status.insertCount++;

        if (count >= size) {
            return false;
        }
        bool isSuccess = false;
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
                // update status
                if (i > 0) {
                    status.insertionCollisions++;
                }
                status.totalInsertProbing += i;
                status.maxInsertProbing = std::max(i, status.maxInsertProbing);
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

        // update status
        if (i > 0) {
            status.insertionCollisions++;
        }
        status.totalInsertProbing += i;
        status.maxInsertProbing = std::max(i, status.maxInsertProbing);

        return true;
    }

    bool remove(uint key) {
        // update status
        status.deleteCount++;
        bool isSuccess = false;
        uint hash = hashFunction(key);
        // linear probing
        uint i = 0;
        while (i < size)  {
            uint idx = (hash + i) & (size - 1);
            // empty slot
            if (!table[idx].isOccupied) {
                break;
                // key found
            } else if (table[idx].key == key) {
                // key found and not deleted
                if (!table[idx].isDeleted) {
                    table[idx].isDeleted = true;
                    count--;
                    isSuccess = true;
                    break;
                }
                // key found but deleted
                break;
            } 
            i++;
        }
        // update status
        status.maxDeleteProbing = std::max(i, status.maxDeleteProbing);
        status.totalDeleteProbing += i;
        // do not found any key
        return isSuccess;
    }

    bool search(uint key) {
        uint hash = hashFunction(key);
        status.searchCount++;
        bool isSuccess = false;
        // linear probing
        uint i = 0;
        while (i < size)  {
            uint idx = (hash + i) & (size - 1);
            // empty slot
            if (!table[idx].isOccupied) {
                break;
                // key found
            } else if (table[idx].key == key) {
                // key found and not deleted
                if (!table[idx].isDeleted) {
                    isSuccess = true;
                    break;
                }
                // key found but deleted
                break;
            }
            i++;
        }
        status.maxSearchProbing = std::max(i, status.maxSearchProbing);
        status.totalSearchProbing += i;
        // do not found any key
        return isSuccess;
    }

    float getCollisionRate() {
        return (1.0f * status.insertionCollisions / status.insertCount);
    }

    unsigned long long getAVGInsertionProbing() {
        return ((status.totalInsertProbing) / status.insertCount);
    }

    unsigned long long getAVGRemoveProbing() {
        return ((status.totalDeleteProbing) / status.deleteCount);
    }

    unsigned long long getAVGSearchProbing() {
        return ((status.totalSearchProbing) / status.searchCount);
    }

    uint getMaxInsertProbing() {
        return status.maxInsertProbing;
    }

    uint getMaxRemoveProbing() {
        return status.maxDeleteProbing;
    }

    uint getMaxSearchProbing() {
        return status.maxSearchProbing;
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