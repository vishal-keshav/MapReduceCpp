/*
 * This is a client application that uses MapReduce library.
 * This client implements map reduce for word counter application.
 */

#include <iostream>
#include <cstdlib>
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

class WordCounterMapReduce: public MapReduceInterface {
public:
    void map_fn(string key, string value) {
        istringstream iss(value);
        string word;
        do {
            iss >> word;
            if(word.size() != 0) {
                emitIntermediate(word, "1");
            }
        } while (iss);
    }

    void reduce_fn(string key, vector<string> values) {
        int count = 0;
        for (int i =0; i<values.size(); i++) {
            count += stoi(values[i]);
        }
        emit(key, vector<string>{to_string(count)});
    }
};


int main() {
    // This is a smaple implementation for word counter application.
    MapReduceMaster masterInstance("WordCounterInput.txt", "WordCounterData");
    MapReduceInterface* map_reduce_func = new WordCounterMapReduce();
    //masterInstance.nr_mapper = 2;
    //masterInstance.nr_reducer = 2;
    int result = masterInstance.process(map_reduce_func);

    // Now interpred the result of MapReduce
    if (result == -1){
        cout << "WordCounter MapReduce failed" << endl;
    } else {
        cout << "WordCounter MapReduce success" << endl;
    }
    return 0;
}