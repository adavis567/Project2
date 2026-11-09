#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstddef>
#include <string>
#include <vector>

class HashTable {
private:
    struct Entry {
        std::string key;
        std::size_t index;
    };

    std::vector<std::vector<Entry>> buckets;
    std::size_t capacity;
    std::size_t entries;
    std::size_t collisions;

    std::size_t hashFunction(const std::string& key) const;
    static std::string normalize(const std::string& key);

public:
    explicit HashTable(std::size_t initialCapacity = 101);
    bool insert(const std::string& key, std::size_t index);
    bool find(const std::string& key, std::size_t& index) const;
    bool contains(const std::string& key) const;
    std::size_t getCapacity() const;
    std::size_t getEntries() const;
    std::size_t getCollisions() const;
};

#endif
