/*
 * This is a client application that uses MapReduce library.
 * This client implements map reduce for ReverseWeblinkGraph application.
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

vector<pair<string, string>> map_fn(string k, string value) {
    //[TODO]
    // pass the file that lists the scraped website source-targets
    // map function accesses each line, splits on comma,
    // returns <target,source>
    vector<pair<string, string>> ret;
    istringstream iss(value);
    while(iss.good()) {
        string source, target;
        getline(getline(iss, source, ','), target);
        ret.push_back(make_pair(target, source));
    }
    return ret;
}

vector<string> reduce_fn(string target, vector<string> source) {
    //[TODO]
    // for a given key (target) this will just create a 
    // list of all of the sources that link to that target
    // I think this could just return the source vector because
    // it is already in the format we need but wrote it out anyway
    vector<string> sources;
    for (int i =0; i<source.size(); i++) {
        sources.push_back(source[i]);
    }
    return sources;
}

int main() {
    // This is a smaple implementation for reverse web-link application.
    MapReduceMaster<string, string, string, string, string> masterInstance("website_source_targets.txt", "ReverseWeblinkGraph", map_fn, reduce_fn);
    int result = masterInstance.process();

    // Now interpret the result of MapReduce
    if (result == -1){
        cout << "Reverse Web-link MapReduce failed" << endl;
    } else {
        cout << "Reverse Web-link MapReduce success" << endl;
    }
    return 0;

}