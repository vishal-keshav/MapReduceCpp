#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility>

#include "Reducer.hpp"
#include "ReducerResult.hpp"

using namespace std;

ReducerResult Reducer::execute(vector<string> tempFileNames,
        string outDirectoryName,
        int (*reduce_fn)(string key, vector<int> values),
        int nr_reducer){
    std::map<string, vector<int>> reducer_key_value_data;
    // Here we know we have only one file to process, so indexing by 0
    ifstream file_temp(tempFileNames[0]);
    string line;
    string word;
    int count;
    while(!file_temp.eof()) {
	    getline(file_temp, line);
	    istringstream iss(line);
        do {
            iss >> word;
            if (word.size() == 0){
                break;
            }
            iss >> count;
            if (reducer_key_value_data.count(word) == 0) {
                vector<int> temp{count};
                reducer_key_value_data.insert(pair<string, vector<int>>(word, temp));
            } else {
                reducer_key_value_data[word].push_back(count);
            }
        } while (iss);
    }
    std::map<string, int> reduced_data;
    for (auto elem : reducer_key_value_data) {
        reduced_data.insert(pair<string, int>(elem.first, reduce_fn(elem.first, elem.second)));
    }
    // Save the reduced_data in outputDir under filename output.txt
    std::ofstream fout("output.txt");
    for (auto elem : reduced_data) {
        fout << elem.first << " " << elem.second;
        fout << '\n';
    }
    ReducerResult res(0);
    return res;
}