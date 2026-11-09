#include "Facility.h"
#include "FileLoader.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

FacilityDatabase::FacilityDatabase() : hashTable(101) {}

bool FacilityDatabase::validID(const string& id) {
    if (id.size() != 4 || (id[0] != 'F' && id[0] != 'f')) return false;
    return isdigit(static_cast<unsigned char>(id[1])) &&
           isdigit(static_cast<unsigned char>(id[2])) &&
           isdigit(static_cast<unsigned char>(id[3]));
}

bool FacilityDatabase::containsIgnoreCase(const vector<string>& values,
                                          const string& value) {
    for (const string& item : values)
        if (FileLoader::lower(item) == FileLoader::lower(value)) return true;
    return false;
}

void FacilityDatabase::loadFacilities(const string& filename,
                                       const vector<string>& buildings) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << ".\n";
        return;
    }

    string line;
    int lineNumber = 0;
    size_t accepted = 0, rejected = 0;

    while (getline(input, line)) {
        ++lineNumber;
        if (FileLoader::isIgnoredLine(line)) continue;

        vector<string> fields = FileLoader::split(line, '|');
        if (fields.size() != 4) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": expected 4 fields.\n";
            ++rejected;
            continue;
        }

        string id = fields[0], name = fields[1], building = fields[2], category = fields[3];
        if (!validID(id) || name.empty() || building.empty() || category.empty()) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": invalid ID or empty field.\n";
            ++rejected;
            continue;
        }

        bool buildingFound = false;
        for (const string& known : buildings)
            if (FileLoader::lower(known) == FileLoader::lower(building)) {
                building = known;
                buildingFound = true;
                break;
            }

        if (!buildingFound) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": building does not exist: " << fields[2] << ".\n";
            ++rejected;
            continue;
        }

        if (hashTable.contains(id)) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": duplicate facility ID " << id << ".\n";
            ++rejected;
            continue;
        }

        id[0] = 'F';
        size_t index = facilities.size();
        if (!hashTable.insert(id, index)) {
            ++rejected;
            continue;
        }

        facilities.push_back({id, name, building, category});
        if (!containsIgnoreCase(categories, category)) categories.push_back(category);
        ++accepted;
    }

    cout << "Facilities: " << accepted << " accepted, "
         << rejected << " rejected.\n";
}

void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facilities loaded.\n";
        return;
    }
    for (const Facility& f : facilities)
        cout << f.id << " | " << f.name << " | " << f.building
             << " | " << f.category << '\n';
}

void FacilityDatabase::searchByID(const string& id) const {
    size_t index = 0;
    if (!hashTable.find(id, index)) {
        cout << "Facility not found.\n";
        return;
    }
    const Facility& f = facilities[index];
    cout << f.id << " | " << f.name << " | " << f.building
         << " | " << f.category << '\n';
}

void FacilityDatabase::searchByName(const string& name) const {
    bool found = false;
    for (const Facility& f : facilities) {
        if (FileLoader::lower(f.name) == FileLoader::lower(FileLoader::trim(name))) {
            cout << f.id << " | " << f.name << " | " << f.building
                 << " | " << f.category << '\n';
            found = true;
        }
    }
    if (!found) cout << "No facility with that complete name was found.\n";
}

void FacilityDatabase::searchByCategory(const string& category) const {
    bool found = false;
    for (const Facility& f : facilities) {
        if (FileLoader::lower(f.category) == FileLoader::lower(FileLoader::trim(category))) {
            cout << f.id << " | " << f.name << " | " << f.building
                 << " | " << f.category << '\n';
            found = true;
        }
    }
    if (!found) cout << "No facilities found in that category.\n";
}

void FacilityDatabase::categoryReport() const {
    cout << "\nFacility Category Report\n";
    cout << "------------------------\n";
    if (categories.empty()) {
        cout << "No facility categories available.\n";
        return;
    }
    for (const string& category : categories) {
        size_t count = 0;
        for (const Facility& f : facilities)
            if (FileLoader::lower(f.category) == FileLoader::lower(category)) ++count;
        cout << category << ": " << count << '\n';
    }
    cout << "Hash table capacity: " << hashTable.getCapacity() << '\n'
         << "Hash table entries: " << hashTable.getEntries() << '\n'
         << "Hash collisions: " << hashTable.getCollisions() << '\n';
}

const vector<Facility>& FacilityDatabase::getFacilities() const { return facilities; }
size_t FacilityDatabase::getHashCapacity() const { return hashTable.getCapacity(); }
size_t FacilityDatabase::getHashEntries() const { return hashTable.getEntries(); }
size_t FacilityDatabase::getHashCollisions() const { return hashTable.getCollisions(); }
