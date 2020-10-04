#pragma once

#include <string>
#include <vector>

class MapperResult {
public:
    MapperResult(int res, std::vector<std::string> tempFiles): result(res),
        tempMapFiles(tempFiles) {}
public:
    int result;
    std::vector<std::string> tempMapFiles;
};
