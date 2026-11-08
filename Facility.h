#ifndef FACILITY_H
#define FACILITY_H

#include <string>
#include <vector>
#include <iostream>
#include "HashTable.h"

using namespace std;

// Stores information about one campus facility.
// Author: Anthony Davis

struct Facility {
    string id;
    string name;
    string building;
    string category;
};

// Manages campus facilities and their searches.
// Author: Anthony Davis

class FacilityDatabase {
private:
    vector<Facility> facilities;
    vector<string> categories;
    HashTable hashTable;

    string trim(const string& text) const;
    string normalize(const string& text) const;
    bool validID(const string& id) const;

public:
    // Constructor
    FacilityDatabase();

    // Loads and validates facility records.
    void loadFacilities(
        const string& filename,
        const vector<string>& buildings
    );

    // Displays all accepted facilities.
    void displayAll() const;

    // Searches facilities using the hash table.
    void searchByID(const string& id) const;

    // Searches facilities by complete name.
    void searchByName(const string& name) const;

    // Searches facilities by category.
    void searchByCategory(const string& category) const;

    // Displays facility category statistics.
    void categoryReport() const;

    // Returns accepted facilities.
    const vector<Facility>& getFacilities() const;

    // Returns hash table statistics.
    size_t getHashCapacity() const;
    size_t getHashEntries() const;
    size_t getHashCollisions() const;
};

#endif
