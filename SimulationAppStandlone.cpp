/**
 * This is a simulation of a client program that by itself simulates mapping
 * and reducing on a test file where each line is considered to be one record.
 * The map and reduce functionality is written for word counter.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility> 

using namespace std;

/*
 * A map function that takes in one record (in out case, it is one line of text,
 * file) and emits a list of key, value pairs. The key will be understood and
 * hashable by master and value is understood by reduce function.
 * 
 */
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

/*
 * A reduce function accepts the key (based on which master hashed and decided 
 * to invoke the reduce on one particular thread) and a list of values which
 * this thread needs to process. The type os key is same as the type produced
 * by map and the type of values is same as type produced by map.
 * 
 */
int reduce_fn(string k, vector<int> values) {
    int ret = 0;
    for (int i =0; i<values.size(); i++) {
        ret += values[i];
    }
    return ret;
}

int main(){
    ifstream file_temp("input.txt");
    string str;
    // First count the number of lines, then invoke mapper on each line (record)
    int sTotal = 0;
    while(!file_temp.eof()) {
	    getline(file_temp, str);
	    sTotal++;
    }
    cout << "Total number of lines in the file is:" << sTotal << endl;
    cout << "Sending each record to one mapper, sequentially" << endl;

    // Below code simulates the mapper.
    int line_number = 0;
    vector<vector<pair<string, int>>> all_processed_records;
    ifstream file("input.txt");
    while (getline(file, str)) {
        vector<pair<string,int>> mapped_record = map_fn(to_string(line_number),str);
        all_processed_records.push_back(mapped_record);
        line_number += 1;
    }

    cout << "All records are processed by the mapper" <<  endl;
    // Below code simulates the reducer.
    cout << "Sending the them all to one reducer, without any hashing" << endl;
    std::map<string, vector<int>> reducer_map;
    for (int i = 0; i < all_processed_records.size(); i++) {
        vector<pair<string, int>> processed_record = all_processed_records[i];
        for (int j = 0; j < processed_record.size(); j++) {
            pair<string, int> p = processed_record[j];
            string key = p.first;
            int count = p.second;
            // Since we have only one reduce thread, this key will go to this
            // only reducer.
            if(reducer_map.count(key) == 0) {
                vector<int> temp{count};
                reducer_map.insert(pair<string, vector<int>>(key, temp));
            } else {
                reducer_map[key].push_back(count);
            }
        }
    }
    // Now process each key in reducer_map
    std::map<string, int> reduced_data;
    for (auto elem : reducer_map) {
        reduced_data.insert(pair<string, int>(elem.first, reduce_fn(elem.first, elem.second)));
    }
    cout << "Reduction done, now prinitng the result" << endl;
    for (auto elem : reduced_data) {
        cout << elem.first << " : " << elem.second << endl;
    }
    return 0;
}