/*
Use map-reduce library to implement the single process version of Inverted Index application. 
It reads the file /data/InvertedIndexInput.txt and lists all positions of a word.
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
// inputs a line number k and string of words
// returns a vector of key-value pairs containing each word and line number.
vector<pair<string, int>> map_fn(string k, string value) { 
    // k is the number of the line (to_string(record number)), value is the line
    vector<pair<string, int>> ret;

    istringstream iss(value);
    do {
        string word;
        iss >> word;
        // loop over each word, push the word with this line number to array
        if(word.size() != 0) {
            // pair of the word and the line it is in
            ret.push_back(make_pair(word, stoi(k)));
        }
    } while (iss);
    return ret;
}

// user defined reduce function
// inputs the intermediate key-values, builds a vector of indexes, sorts and removes duplicates
// returns vector of unique indexes in sorted order
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