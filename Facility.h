#ifndef FACILITY_H
#define FACILITY_H

#include "HashTable.h"
#include <string>
#include <vector>

struct Facility {
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

class FacilityDatabase {
private:
    std::vector<Facility> facilities;
    std::vector<std::string> categories;
    HashTable hashTable;

    static bool validID(const std::string& id);
    static bool containsIgnoreCase(const std::vector<std::string>& values,
                                   const std::string& value);

public:
    FacilityDatabase();
    void loadFacilities(const std::string& filename,
                        const std::vector<std::string>& buildings);
    void displayAll() const;
    void searchByID(const std::string& id) const;
    void searchByName(const std::string& name) const;
    void searchByCategory(const std::string& category) const;
    void categoryReport() const;
    const std::vector<Facility>& getFacilities() const;
    std::size_t getHashCapacity() const;
    std::size_t getHashEntries() const;
    std::size_t getHashCollisions() const;
};

#endif
