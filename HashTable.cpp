#include "HashTable.h"
#include <cctype>

HashTable::HashTable(std::size_t initialCapacity)
    : capacity(initialCapacity == 0 ? 101 : initialCapacity),
      entries(0), collisions(0) {
    buckets.resize(capacity);
}

std::string HashTable::normalize(const std::string& key) {
    std::string result = key;
    for (char& c : result)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return result;
}

std::size_t HashTable::hashFunction(const std::string& key) const {
    std::size_t hash = 0;
    for (unsigned char c : key)
        hash = (hash * 31 + c) % capacity;
    return hash;
}

bool HashTable::insert(const std::string& key, std::size_t index) {
    const std::string normalized = normalize(key);
    const std::size_t bucket = hashFunction(normalized);
    for (const Entry& entry : buckets[bucket])
        if (entry.key == normalized) return false;

    if (!buckets[bucket].empty()) ++collisions;
    buckets[bucket].push_back({normalized, index});
    ++entries;
    return true;
}

bool HashTable::find(const std::string& key, std::size_t& index) const {
    const std::string normalized = normalize(key);
    const std::size_t bucket = hashFunction(normalized);
    for (const Entry& entry : buckets[bucket]) {
        if (entry.key == normalized) {
            index = entry.index;
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const std::string& key) const {
    std::size_t index = 0;
    return find(key, index);
}

std::size_t HashTable::getCapacity() const { return capacity; }
std::size_t HashTable::getEntries() const { return entries; }
std::size_t HashTable::getCollisions() const { return collisions; }
