#include "HashTable.h"
#include <cctype>

// Author: Anthony Davis

// Creates the hash table with the specified capacity.
HashTable::HashTable(size_t initialCapacity)
    : capacity(initialCapacity == 0 ? 101 : initialCapacity),
      entries(0),
      collisions(0) {
    table.resize(capacity);
}

// Calculates the hash index using the characters of the ID.
size_t HashTable::hashFunction(const string& key) const {
    size_t hash = 0;

    for (char character : key) {
        hash = (hash * 31 + static_cast<unsigned char>(character)) % capacity;
    }

    return hash;
}

// Inserts a normalized facility ID into the table.
bool HashTable::insert(const string& key, size_t index) {
    string normalized = key;

    for (char& character : normalized) {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character))
        );
    }

    size_t position = hashFunction(normalized);

    for (const Entry& entry : table[position]) {
        if (entry.key == normalized) {
            return false;
        }
    }

    if (!table[position].empty()) {
        collisions++;
    }

    table[position].push_back({normalized, index});
    entries++;

    return true;
}

// Searches for a normalized facility ID.
bool HashTable::find(const string& key, size_t& index) const {
    string normalized = key;

    for (char& character : normalized) {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character))
        );
    }

    size_t position = hashFunction(normalized);

    for (const Entry& entry : table[position]) {
        if (entry.key == normalized) {
            index = entry.index;
            return true;
        }
    }

    return false;
}

// Checks whether an ID exists.
bool HashTable::contains(const string& key) const {
    size_t index;
    return find(key, index);
}

// Returns the table capacity.
size_t HashTable::getCapacity() const {
    return capacity;
}

// Returns the number of stored entries.
size_t HashTable::getEntries() const {
    return entries;
}

// Returns the number of collisions.
size_t HashTable::getCollisions() const {
    return collisions;
}
