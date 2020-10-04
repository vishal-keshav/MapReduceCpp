#include <iostream>
#include <string>

#include "MapReduceMaster.hpp"
#include "MapReduceResult.hpp"
#include "Mapper.hpp"
#include "Reducer.hpp"
#include "MapperResult.hpp"
#include "ReducerResult.hpp"

using namespace std;

MapReduceMaster::MapReduceMaster() {
    // Do nothing in constructor
}

MapReduceResult MapReduceMaster::process(MapReduceConfiguration config) {
    // Execute mapper, get the result
    MapperResult mapperResult = mapper.execute(config.inputFileName,
        config.map_fn, config.nr_mapper);
    ReducerResult reducerResult = reducer.execute(mapperResult.tempMapFiles,
        config.outputResultDirectory, config.reduce_fn, config.nr_reducer);
    MapReduceResult res(0);
    return res;
}