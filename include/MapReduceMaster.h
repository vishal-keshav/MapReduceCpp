/*
 * The header file contains the implmentation of MapReduceMaster and mapper/reducer
 * workers. The class accepts a generic map/reduce UDF function from the client
 * program. We support only one text file where each line in the text file is
 * considered to be on record. The text file can have any data type, like string,
 * int, float etc. Refer sample programs to understand the usage of APIs.
 * 
 * The current implementation uses a single thread for mapper and a single thread
 * for reducer. This is sufficient for the first deadline submission.
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <map>
#include <utility>

#include "Utility.h"

/*
 * MapReduceInterface is sub-classes by client where they implement two virtual
 * functions namely map_fn and reduce_fn. The APIs for map_fn and reduce_fn
 * is standardized. The signatature is explained in the function definition.
 */
class MapReduceInterface {
public:
    virtual void map_fn(string k1, string v1) = 0;
    virtual void reduce_fn(string k2, vector<string> v2_list) = 0;
protected:
    void emitIntermediate(string k2, string v2) {
        emitted_intermediates.push_back(make_pair(k2, v2));
    }
    void emit(string k2, vector<string> v3_list) {
        emitted_outputs.push_back(make_pair(k2, v3_list));
    }
public:
    vector<pair<string, string>> emitted_intermediates;
    vector<pair<string, vector<string>>> emitted_outputs;
};

int map_controller_module(string inputFileName, MapReduceInterface *map_reduce_fn, int nr_mapper);
int reduce_controller_module(string outputResultDirectory, MapReduceInterface *map_reduce_fn, int nr_reducer);

/*
 * This is the main class that client instantiate in order to do map reduce.
 */
class MapReduceMaster {
public:
    MapReduceMaster(string f_name, string out_dir): inputFileName(f_name), outputResultDirectory(out_dir){}

    int process(MapReduceInterface *map_reduce_fn) {
        // Call the map_contoller, wait for the acknowledgement.
        int map_ack = map_controller_module(this->inputFileName, map_reduce_fn, this->nr_mapper);

        // Call the reduce_controller, wait for the acknowlegement.
        int reduce_ack = reduce_controller_module(this->outputResultDirectory, map_reduce_fn, this->nr_reducer);
        return 0;
    }

public:
    std::string inputFileName;
    std::string outputResultDirectory;
    int nr_mapper{1};
    int nr_reducer{1};
};

// Hashes a data type (int, float, double, string) in the range of 0 to max_range-1
template<typename d>
int hash_in_range(d data, int max_range) {
    hash<d> hasher;
    return hasher(data)%max_range;
}

int map_controller_module(string inputFileName, MapReduceInterface *map_reduce_fn, int nr_mapper) {
    int record_number = 0;
    vector<vector<pair<string, string>>> all_processed_records;
    ifstream file(inputFileName);
    string str;
    while (getline(file, str)) {
        map_reduce_fn->map_fn(to_string(record_number),str);
        record_number += 1;
    }
    ofstream fout("temp.txt");

    for (auto elem : map_reduce_fn->emitted_intermediates) {
        fout << elem.first << " " << elem.second << '\n';
    }
    fout.close();
    return 0;
}


int reduce_controller_module(string outputResultDirectory, MapReduceInterface *map_reduce_fn, int nr_reducer) {
    string tempFile = "temp.txt";
    map<string, vector<string>> reducer_key_value_data = read_text<string, string>(tempFile);
    map<string, vector<string>> reducer_key_value_data_filtered;
    for (auto elem : reducer_key_value_data) {
        // Extend the vector<v2> on k2 if already exists
        if (reducer_key_value_data_filtered.count(elem.first) == 0) {
            reducer_key_value_data_filtered.insert(pair<string, vector<string>>(elem.first, elem.second));
        } else {
            reducer_key_value_data_filtered[elem.first].insert(reducer_key_value_data_filtered[elem.first].end(), elem.second.begin(), elem.second.end());
        }
    }
    for (auto elem : reducer_key_value_data_filtered) {
        map_reduce_fn->reduce_fn(elem.first, elem.second);
    }
    write_key_val_vector(outputResultDirectory + "_output.txt", map_reduce_fn->emitted_outputs);

}
