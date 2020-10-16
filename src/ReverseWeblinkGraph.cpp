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

// User defined function for mapper for reverse web-link graph
// inputs a line number k and string corresponding to a source-target value
// returns <target,source>
vector<pair<string, string>> map_fn(string k, string value) {
    vector<pair<string, string>> ret;
    istringstream iss(value);
    // loop over each line, split on comma, and push key-value pair to output
    while(iss.good()) {
        string source, target;
        getline(getline(iss, source, ','), target);
        ret.push_back(make_pair(target, source));
    }
    return ret;
}

// User defined function for reducer for reverse web-link graph
// inputs a given key (target) and vector source of sources
// returns a vector of all sources associated with target
vector<string> reduce_fn(string target, vector<string> source) {
    // This could just return the source vector because
    // it is already in the format we need but we wrote it out anyway to be explicit
    vector<string> sources;
    // loop over sources in source vector, push them to new sources vector
    for (int i =0; i<source.size(); i++) {
        sources.push_back(source[i]);
    }
    return sources;
}

class WordCounterMapReduce: public MapReduceInterface {
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

int main() {
    // This is a sample implementation for reverse web-link application.
    MapReduceMaster masterInstance("website_source_targets.txt", "ReverseWeblinkGraph");
    MapReduceInterface *func = new WordCounterMapReduce();
    int result = masterInstance.process(func);

    // Now interpret the result of MapReduce
    if (result == -1){
        cout << "Reverse Web-link MapReduce failed" << endl;
    } else {
        cout << "Reverse Web-link MapReduce success" << endl;
    }
    return 0;

}