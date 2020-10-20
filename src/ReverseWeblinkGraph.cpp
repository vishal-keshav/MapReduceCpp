/*
 * This is a client application that uses MapReduce library.
 * This client implements map reduce for ReverseWeblinkGraph application.
 * Mapper takes a file with a list of source-targets in format "[source],[target]"
 * Reducer returs a file with a list of targets and all of the sources that point to them
 * in format "target source1 [source2 ...]"
 */

#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility>

#include "MapReduceMaster.h"

using namespace std;

class ReverseWeblinkGraphMapReduce: public MapReduceInterface {
public:
    void map_fn(string key, string value) {
        istringstream iss(value);
        // loop over each line, split on comma, and push key-value pair to output
        while(iss.good()) {
            string source, target;
            getline(getline(iss, source, ','), target);
            emitIntermediate(target, source);
        }
    }

    void reduce_fn(string key, vector<string> values) {
        vector<string> sources;
        // loop over sources in source vector, push them to new sources vector
        for (int i =0; i<values.size(); i++) {
            sources.push_back(values[i]);
        }
        emit(key, sources);
    }
};

MapReduceInterfaceFactoryRegistration<ReverseWeblinkGraphMapReduce> _ReverseWeblinkGraphMapReduce("MapReduce");

int main() {
    // This is a sample implementation for reverse web-link application.
    MapReduceMaster masterInstance("ReverseWeblinkGraphInput.txt", "ReverseWeblinkGraphData");
    int result = masterInstance.process();

    // Now interpret the result of MapReduce
    if (result == -1){
        cout << "Reverse Web-link MapReduce failed" << endl;
    } else {
        cout << "Reverse Web-link MapReduce success" << endl;
    }
    return 0;

}