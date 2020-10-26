/*
Use map-reduce library to implement the single process version of Inverted Index application. 
It reads the file /data/InvertedIndexInput.txt and lists all positions of a word.
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "MapReduceMaster.h"

using namespace std;

class InvertedIndexMapReduce: public MapReduceInterface {
public:
    void map_fn(string key, string value) {
        stringstream iss(value);
        string word;
        while (iss >> word) {
            emitIntermediate(word, key);
        }
    }

    void reduce_fn(string key, vector<string> values) {
        vector<string> v3;
        for (int i = 0; i < values.size(); i++){
            v3.push_back(values[i]);
        }
        sort(v3.begin(), v3.end());
        v3.erase(unique(v3.begin(), v3.end()), v3.end());
        emit(key, v3);
    }
};

MapReduceInterfaceFactoryRegistration<InvertedIndexMapReduce> _InvertedIndexMapReduce("MapReduce");

int main() {
    // set the parameters by configuration
    int nr_workers;
    string inputFileName;
    string dataDirectory;
    ifstream file("config_InvertedIndex.txt");
    string str;
    while (getline(file, str)) {
        istringstream in(str);
        vector<string> v;
        string s;
        while (in >> s) {
            v.push_back(s);
        }
        if (v[0] == "N_WORKER") nr_workers = stoi(v[1]);
        else if (v[0] == "INPUTFILE") inputFileName = v[1];
        else if (v[0] == "DATADIR") dataDirectory = v[1];
        else;
    }

    // Create master instance.
    MapReduceMaster masterInstance(inputFileName, dataDirectory, nr_workers);
    int result = masterInstance.process();

    // Now interpred the result of MapReduce
    if (result == -1){
        cout << "InvertedIndex MapReduce failed" << endl;
    } else {
        cout << "InvertedIndex MapReduce success" << endl;
    }
    return 0;
}