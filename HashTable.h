#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <cstddef>

using namespace std;

// Custom hash table using separate chaining.
// Author: Anthony Davis

class HashTable {
private:
    struct Entry {
        string key;
        size_t index;
    };

    vector<vector<Entry>> table;
    size_t capacity;
    size_t entries;
    size_t collisions;

    size_t hashFunction(const string& key) const;

public:
    // Constructor
    HashTable(size_t initialCapacity = 101);

    // Inserts an ID and its facility position.
    bool insert(const string& key, size_t index);

    // Searches for an ID.
    bool find(const string& key, size_t& index) const;

    // Checks whether an ID exists.
    bool contains(const string& key) const;

    // Returns table statistics.
    size_t getCapacity() const;
    size_t getEntries() const;
    size_t getCollisions() const;
};

#endif
