/*
 * This is a client application that uses MapReduce library.
 * The application this client implements through map reduce is word counter.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility>

#include "MapReduceMaster.h"

using namespace std;

// User defined map function for word counter application
vector<pair<string, int>> map_fn(string k, string value) {
    vector<pair<string, int>> ret;
    // Ignore the key, typically this tells which record(text line) are we 
    // processing right now.
    istringstream iss(value);
    do {
        string word;
        iss >> word;
        if(word.size() != 0) {
            ret.push_back(make_pair(word, 1));
        }
    } while (iss);
    return ret;
}

// User defined reduce function for word counter application
vector<int> reduce_fn(string k, vector<int> values) {
    vector<int> ret;
    int count = 0;
    for (int i =0; i<values.size(); i++) {
        count += values[i];
    }
    ret.push_back(count);
    return ret;
}


int main() {
    MapReduceMaster<string, string, string, int, int> masterInstance("input.txt", ".", map_fn, reduce_fn);
    int result = masterInstance.process();

    // Now interpred the result of MapReduce
    if (result == -1){
        cout << "MapReduce failed" << endl;
    } else {
        cout << "MapReduce success" << endl;
    }


    return 0;
}