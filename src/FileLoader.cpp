#include "FileLoader.h"
#include <algorithm>
#include <cctype>

namespace FileLoader {
    std::string trim(const std::string& value) {
        const std::string whitespace = " \t\r\n";
        const std::size_t first = value.find_first_not_of(whitespace);
        if (first == std::string::npos) return "";
        const std::size_t last = value.find_last_not_of(whitespace);
        return value.substr(first, last - first + 1);
    }

    std::string lower(const std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return result;
    }

    std::vector<std::string> split(const std::string& line, char delimiter) {
        std::vector<std::string> fields;
        std::size_t start = 0;
        while (true) {
            std::size_t position = line.find(delimiter, start);
            if (position == std::string::npos) {
                fields.push_back(trim(line.substr(start)));
                break;
            }
            fields.push_back(trim(line.substr(start, position - start)));
            start = position + 1;
        }
        return fields;
    }

    bool isIgnoredLine(const std::string& line) {
        std::string cleaned = trim(line);
        return cleaned.empty() || cleaned[0] == '#';
    }
}

