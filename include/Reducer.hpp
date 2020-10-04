#pragma once

#include <string>
#include <vector>

#include "ReducerResult.hpp"

class Reducer {
public:
    ReducerResult execute(std::vector<std::string> tempFileNames,
        std::string outDirectoryName, 
        int (*r_fn)(std::string k, std::vector<int> values), int nr_reducer);
};