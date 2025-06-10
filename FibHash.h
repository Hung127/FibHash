#ifndef FIB_HASH_H
#define FIB_HASH_H
#include <iostream>
#include <vector>
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

class FibonacciHashTable {
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
    // sizeLog = log2(size)
    uint sizeLog;
    // number of keys
    uint count;
    // hash table
    std::vector<Entry> table;

    uint hashFunction(uint key) {
        // use ((unsigned long long) key * FIB_CONST) & ((1ull << WORD_SIZE) - 1) to make sure 
        // that it will always fits int 32 unsigned bits (number <= 2^32 - 1)
        return (((unsigned long long) key * FIB_CONST) & ((1ull << WORD_SIZE) - 1)) >> (WORD_SIZE - sizeLog);
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
        if (2 * count >= size) {
            resize(size * 2, sizeLog + 1);
        }

        uint hash = hashFunction(key);
        bool firstDeleted = false;
        uint firstDeletedIdx = 0;

        if (!table[hash].isOccupied) {
            table[hash].key = key;
            table[hash].isOccupied = true;
            table[hash].isDeleted = false;
            count++;
            return true;
        } else if (table[hash].isDeleted) {
            firstDeleted = true;
            firstDeletedIdx = hash;
        }

        // linear probing
        uint i = (hash + 1) & (size - 1);
        for (; i != hash; i = (i + 1) & (size - 1)) {
            // found empty slot
            if (!table[i].isOccupied)
                break;
            else if (table[i].isDeleted) {
                if (!firstDeleted) {
                    firstDeletedIdx = i;
                    firstDeleted = true;
                }
                // key already existed (duplicated) and not deleted
            } else if (table[i].key == key) {
                return false;
            }
        }

        // make sure that always add key to deleted place first (if can)
        if (firstDeleted) {
            i = firstDeletedIdx;
        }

        // add to empty slot or deleted slot
        table[i].key = key;
        table[i].isDeleted = false;
        table[i].isOccupied = true;
        count++;
        return true;
    }

    bool remove(uint key) {
        uint hash = hashFunction(key);
        // empty slot
        if (!table[hash].isOccupied) {
            return false;
        } else if (table[hash].isDeleted && table[hash].key == key) {
            return false;
            // found
        } else if (!table[hash].isDeleted && table[hash].key == key) {
            count--;
            table[hash].isDeleted = true;
            return true;
        }
        for (uint i = (hash + 1) & (size - 1); i != hash; i = (i + 1) & (size - 1)) {
            if (!table[i].isOccupied) {
                return false;
                // key found but deleted
            } else if (table[i].key == key) {
                if (!table[i].isDeleted) {
                    table[i].isDeleted = true;
                    count--;
                    return true;
                }
                return false;
            } 
        }
        // do not found any key
        return false;
    }

    bool search(uint key) {
        uint hash = hashFunction(key);
        // empty slot
        if (!table[hash].isOccupied) {
            return false;
        } else if (table[hash].isDeleted && table[hash].key == key) {
            return false;
            // found
        } else if (!table[hash].isDeleted && table[hash].key == key) {
            return true;
        }
        for (uint i = (hash + 1) & (size - 1); i != hash; i = (i + 1) & (size - 1)) {
            if (!table[i].isOccupied) {
                return false;
                // key found but deleted
            } else if (table[i].key == key) {
                if (!table[i].isDeleted)
                    return true;
                return false;
            } 
        }
        // do not found any key
        return false;
    }

    // assume that tableSize is always be a power of 2
    FibonacciHashTable(uint tableSize) : size(std::max(1u, tableSize)), sizeLog(getLog2(tableSize)), table(std::max(1u, tableSize)), count(0) {
        // int temp = sizeLog;
        // sizeLog = 3;
        // for (uint i = 0; i < 10; i++) {
        //     std::cout << hashFunction(i) << " " << i << "\n";
        // }
        // sizeLog = temp;
    }
};

#endif