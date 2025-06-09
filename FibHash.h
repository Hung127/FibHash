#ifndef FIB_HASH
#define FIB_HASH
#include <iostream>
#include <vector>
#define uint unsigned int
#define FIB_CONST 2654435759
#define WORD_SIZE 32

uint getLog2(uint num) {
    uint count = 0;
    while (num > 0) {
        num = num >> 1;
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
    // sizeLog = log2(size)
    uint sizeLog;
    // number of keys
    uint count;
    // hash table
    std::vector<Entry> table;

    uint hashFunction(uint key) {
        return ((unsigned long long) key * FIB_CONST) >> (WORD_SIZE - sizeLog);
    }

public:
    bool insert(uint key) {
        if (count >= size) {
            return false;
        }
        uint hash = hashFunction(key);
        bool firstDeleted = false;
        uint firstDel;
        if (!table[hash].isOccupied) {
            table[hash].key = key;
            table[hash].isOccupied = true;
            return true;
        } else if (table[hash].isDeleted) {
            firstDeleted = true;
            firstDel = hash;
        }

        // linear probing
        uint i = (hash + 1) & size;
        for (; i != hash; i = (i + 1) & size) {
            // found empty slot
            if (!table[i].isOccupied)
                break;
            else if (table[i].isDeleted && !firstDeleted) {
                if (!firstDeleted) {
                    firstDel = i;
                    firstDeleted = true;
                }
                // key already existed
            } else if (table[i].key == key) {
                return false;
            }
        }

        // make sure that always add key to deleted place first (if can)
        if (firstDeleted) i = firstDel;

        table[i].key = key;
        table[i].isDeleted = false;
        table[i].isOccupied = true;
        return true;
    }

    HashTable(uint tableSize) : size(tableSize), sizeLog(getLog2(tableSize)), table(1 >> sizeLog), count(0) {}
};

#endif