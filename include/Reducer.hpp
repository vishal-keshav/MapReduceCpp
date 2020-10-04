#pragma once

#include <string>
#include <vector>

#include "ReducerResult.hpp"

class Reducer {
public:
    ReducerResult execute(std::vector<std::string> tempFileNames,
        std::string outDirectoryName, 
        std::vector<std::pair<std::string, int>> (*r_fn)(
            std::string key, std::string value
        ), int nr_reducer);
};