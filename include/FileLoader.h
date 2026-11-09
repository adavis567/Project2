#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>

namespace FileLoader {
    std::string trim(const std::string& value);
    std::string lower(const std::string& value);
    std::vector<std::string> split(const std::string& line, char delimiter);
    bool isIgnoredLine(const std::string& line);
}

#endif

