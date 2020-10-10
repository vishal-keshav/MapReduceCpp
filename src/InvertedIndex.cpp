/*
Use map-reduce library to implement the single process version of Inverted Index application. 
It reads the file /data/input_InvInd.txt and lists all positions of a word in .
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "MapReduceMaster.h"

using namespace std;


// user defined map function
vector<pair<string, int>> map_fn(string k, string value) { 
    // k is the number of the line (to_string(record number)), value is the line
    vector<pair<string, int>> ret;

    istringstream iss(value);
    do {
        string word;
        iss >> word;
        if(word.size() != 0) {
            // pair of the word and the line it is in
            ret.push_back(make_pair(word, stoi(k)));
        }
    } while (iss);
    return ret;
}

// user defined reduce function
vector<int> reduce_fn(string k, vector<int> values) {
    vector<int> ret;
    for (int i = 0; i < values.size(); i++){
        ret.push_back(values[i]);
    }
    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

int main() {
    // Create master instance.
    MapReduceMaster<string, string, string, int, int> masterInstance("InvertedIndexInput.txt", "InvertedIndex", map_fn, reduce_fn);
    int result = masterInstance.process();

    // Now interpred the result of MapReduce
    if (result == -1){
        cout << "InvertedIndex MapReduce failed" << endl;
    } else {
        cout << "InvertedIndex MapReduce success" << endl;
    }
    return 0;
}