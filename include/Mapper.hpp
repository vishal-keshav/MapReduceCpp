#pragma once

#include <string>
#include <vector>

#include "MapperResult.hpp"

/*
 * A very basic implementation (non-threaded version) of mapper.
 */

class Mapper {
public:
    MapperResult execute(std::string inputFileName, 
        std::vector<std::pair<std::string, int>> (*m_fn)(
            std::string key, std::string value
        ), int nr_mapper);
};