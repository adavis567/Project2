#include "Facility.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

// Author: Anthony Davis

// Initializes the facility database.
FacilityDatabase::FacilityDatabase() : hashTable(101) {
}

// Removes whitespace from the beginning and end of a string.
string FacilityDatabase::trim(const string& text) const {
    size_t start = text.find_first_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    size_t end = text.find_last_not_of(" \t\r\n");

    return text.substr(start, end - start + 1);
}

// Converts a string to uppercase.
string FacilityDatabase::normalize(const string& text) const {
    string result = trim(text);

    for (char& character : result) {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character))
        );
    }

    return result;
}

// Checks whether an ID follows the F followed by three digits format.
bool FacilityDatabase::validID(const string& id) const {
    if (id.length() != 4 || id[0] != 'F') {
        return false;
    }

    for (int i = 1; i < 4; i++) {
        if (!isdigit(static_cast<unsigned char>(id[i]))) {
            return false;
        }
    }

    return true;
}

// Loads facility records from facilities.txt.
void FacilityDatabase::loadFacilities(
    const string& filename,
    const vector<string>& buildings
) {
    ifstream file(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected"
             << " (hash capacity: " << hashTable.getCapacity()
             << ", entries: " << hashTable.getEntries()
             << ", collisions: " << hashTable.getCollisions()
             << ")" << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        lineNumber++;

        string cleaned = trim(line);

        if (cleaned.empty()) {
            continue;
        }

        if (cleaned[0] == '#') {
            continue;
        }

        vector<string> fields;
        string field;
        stringstream stream(line);

        while (getline(stream, field, '|')) {
            fields.push_back(trim(field));
        }

        if (!line.empty() && line.back() == '|') {
            fields.push_back("");
        }

        if (fields.size() != 4) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Incorrect number of fields." << endl;
            rejected++;
            continue;
        }

        bool emptyField = false;

        for (const string& value : fields) {
            if (value.empty()) {
                emptyField = true;
                break;
            }
        }

        if (emptyField) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Empty required field." << endl;
            rejected++;
            continue;
        }

        string id = normalize(fields[0]);
        string name = fields[1];
        string building = fields[2];
        string category = fields[3];

        if (!validID(id)) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Invalid ID format. ID must be F followed by exactly three digits."
                 << endl;
            rejected++;
            continue;
        }

        string acceptedBuilding = "";

        for (const string& existingBuilding : buildings) {
            if (normalize(existingBuilding) == normalize(building)) {
                acceptedBuilding = existingBuilding;
                break;
            }
        }

        if (acceptedBuilding.empty()) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Unknown building '" << building << "'."
                 << endl;
            rejected++;
            continue;
        }

        if (hashTable.contains(id)) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Duplicate facility ID " << id << "."
                 << endl;
            rejected++;
            continue;
        }

        string acceptedCategory = category;

        for (const string& existingCategory : categories) {
            if (normalize(existingCategory) == normalize(category)) {
                acceptedCategory = existingCategory;
                break;
            }
        }

        if (acceptedCategory == category) {
            bool categoryExists = false;

            for (const string& existingCategory : categories) {
                if (normalize(existingCategory) == normalize(category)) {
                    categoryExists = true;
                    break;
                }
            }

            if (!categoryExists) {
                categories.push_back(category);
            }
        }

        Facility facility;

        facility.id = id;
        facility.name = name;
        facility.building = acceptedBuilding;
        facility.category = acceptedCategory;

        size_t facilityIndex = facilities.size();

        if (hashTable.insert(id, facilityIndex)) {
            facilities.push_back(facility);
            accepted++;
        }
    }

    file.close();

    cout << filename << ": "
         << accepted << " accepted, "
         << rejected << " rejected"
         << " (hash capacity: " << hashTable.getCapacity()
         << ", entries: " << hashTable.getEntries()
         << ", collisions: " << hashTable.getCollisions()
         << ")" << endl;
}

// Displays every accepted facility.
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facilities are available." << endl;
        return;
    }

    cout << "\n--- All Facilities ---\n";

    for (const Facility& facility : facilities) {
        cout << facility.id << " | "
             << facility.name << " | "
             << facility.building << " | "
             << facility.category << endl;
    }

    cout << "Total facilities: " << facilities.size() << endl;
}

// Searches for a facility by ID using the custom hash table.
void FacilityDatabase::searchByID(const string& input) const {
    string id = normalize(input);

    if (!validID(id)) {
        cout << "Invalid format: an ID is F followed by exactly three digits."
             << endl;
        return;
    }

    size_t index;

    if (hashTable.find(id, index)) {
        const Facility& facility = facilities[index];

        cout << "Found: "
             << facility.name << " | "
             << facility.building << " | "
             << facility.category << endl;
    }
    else {
        cout << "Not found: no facility with ID "
             << id << endl;
    }
}

// Searches by complete facility name, ignoring capitalization.
void FacilityDatabase::searchByName(const string& input) const {
    string name = normalize(input);

    if (name.empty()) {
        cout << "Error: Facility name cannot be blank." << endl;
        return;
    }

    int matches = 0;

    for (const Facility& facility : facilities) {
        if (normalize(facility.name) == name) {
            cout << facility.id << " | "
                 << facility.name << " | "
                 << facility.building << " | "
                 << facility.category << endl;

            matches++;
        }
    }

    if (matches == 0) {
        cout << "No facilities found with that name." << endl;
    }

    cout << "Matches: " << matches << endl;
}

// Searches by complete category, ignoring capitalization.
void FacilityDatabase::searchByCategory(const string& input) const {
    string category = normalize(input);

    if (category.empty()) {
        cout << "Error: Category cannot be blank." << endl;
        return;
    }

    int matches = 0;

    for (const Facility& facility : facilities) {
        if (normalize(facility.category) == category) {
            cout << facility.id << " | "
                 << facility.name << " | "
                 << facility.building << " | "
                 << facility.category << endl;

            matches++;
        }
    }

    if (matches == 0) {
        cout << "No facilities found in that category." << endl;
    }

    cout << "Matches: " << matches << endl;
}

// Displays category counts and percentages.
void FacilityDatabase::categoryReport() const {
    if (facilities.empty()) {
        cout << "No facilities are available." << endl;
        cout << "Total: 0" << endl;
        return;
    }

    cout << "\n--- Facility Category Report ---\n";

    cout << left << setw(30) << "Category"
         << setw(12) << "Count"
         << "Percentage" << endl;

    for (const string& category : categories) {
        int count = 0;

        for (const Facility& facility : facilities) {
            if (normalize(facility.category) == normalize(category)) {
                count++;
            }
        }

        double percentage =
            (static_cast<double>(count) / facilities.size()) * 100.0;

        cout << left << setw(30) << category
             << setw(12) << count
             << fixed << setprecision(1)
             << percentage << "%" << endl;
    }

    cout << left << setw(30) << "Total"
         << setw(12) << facilities.size()
         << "100.0%" << endl;
}

// Returns the facility vector.
const vector<Facility>& FacilityDatabase::getFacilities() const {
    return facilities;
}

// Returns hash table capacity.
size_t FacilityDatabase::getHashCapacity() const {
    return hashTable.getCapacity();
}

// Returns hash table entries.
size_t FacilityDatabase::getHashEntries() const {
    return hashTable.getEntries();
}

// Returns hash table collisions.
size_t FacilityDatabase::getHashCollisions() const {
    return hashTable.getCollisions();
}
