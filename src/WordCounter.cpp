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

class WordCounterMapReduce: public MapReduceInterface {
public:
    void map_fn(string key, string value) {
        stringstream iss(value);
        string word;
        while (iss >> word) {
            emitIntermediate(word, "1");
        }
    }

    void reduce_fn(string key, vector<string> values) {
        int count = 0;
        for (int i =0; i<values.size(); i++) {
            count += stoi(values[i]);
        }
        emit(key, vector<string>{to_string(count)});
    }
};

MapReduceInterfaceFactoryRegistration<WordCounterMapReduce> _WordCounterMapReduce("MapReduce");

int main() {
    // This is a smaple implementation for word counter application.
    MapReduceMaster masterInstance("WordCounterInput.txt", "WordCounterData", 2);
    int result = masterInstance.process();

    // Now interpred the result of MapReduce
    if (result == -1){
        cout << "WordCounter MapReduce failed" << endl;
    } else {
        cout << "WordCounter MapReduce success" << endl;
    }
    return 0;
}