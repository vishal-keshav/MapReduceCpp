#pragma once

#include "MapReduceResult.hpp"
#include "MapReduceConfiguration.hpp"
#include "Mapper.hpp"
#include "Reducer.hpp"

/*
 * This is the main class that client instantiate in order to do map reduce.
 */

class MapReduceMaster {
public:
    // Constructor, does nothing
    MapReduceMaster(void);
    // main function which clinets invoke. This is a blocking call,
    // hence, clinets should invoke this method in a thread if possible.
    MapReduceResult process(MapReduceConfiguration);
private:
    Mapper mapper;
    Reducer reducer;
};