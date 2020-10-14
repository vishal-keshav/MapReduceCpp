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
#include <string>
#include <thread>
#include <functional>

#include "Utility.h"

// Declaration of map_worker
template<typename k1, typename v1, typename k2, typename v2, typename v3>
int map_contoller(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1), int nr_mapper);

template<typename k1, typename v1, typename k2, typename v2, typename v3>
void map_worker(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1), int thread_id, int nr_mapper);

// Declaration of reduce_worker
template<typename k1, typename v1, typename k2, typename v2, typename v3>
int reduce_controller(string outputResultDirectory, vector<v3> (*reduce_fn)(k2, vector<v2>), int nr_reducer);

template<typename k1, typename v1, typename k2, typename v2, typename v3>
void reduce_worker(string outputResultDirectory, vector<v3> (*reduce_fn)(k2, vector<v2>), int thread_id, int nr_reducer);

/*
 * This is the main class that client instantiate in order to do map reduce.
 */
template<typename k1, typename v1, typename k2, typename v2, typename v3>
class MapReduceMaster {
public:
    MapReduceMaster(
        std::string f_name,
        std::string out_dir,
        std::vector<std::pair<k2, v2>> (*m_fn)(k1, v1),
        std::vector<v3> (*r_fn)(k2, std::vector<v2>)
        ): inputFileName(f_name), outputResultDirectory(out_dir),
          map_fn(m_fn), reduce_fn(r_fn){}

    int process() {
        // Call the map_contoller, wait for the acknowledgement.
        int map_ack = map_contoller<k1,v1,k2,v2,v3>(this->inputFileName, this->map_fn, this->nr_mapper);

        // Call the reduce_controller, wait for the acknowlegement.
        int reduce_ack = reduce_controller<k1,v1,k2,v2,v3>(this->outputResultDirectory, this->reduce_fn, this->nr_reducer);
        return 0;
    }

public:
    std::string inputFileName;
    std::string outputResultDirectory;
    std::vector<std::pair<k2, v2>> (*map_fn)(k1, v1);
    std::vector<v3> (*reduce_fn)(k2, std::vector<v2>);
    int nr_mapper{1};
    int nr_reducer{1};
};

// Hashes a data type (int, float, double, string) in the range of 0 to max_range-1
template<typename d>
int hash_in_range(d data, int max_range) {
    hash<d> hasher;
    return hasher(data)%max_range;
}

template<typename k1, typename v1, typename k2, typename v2, typename v3>
int map_contoller(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1),
        int nr_mapper) {
    // Step 1: Spawn nr_mapper thread to do the map_work
    vector<thread> map_worker_threads;
    for (int i=0; i<nr_mapper; i++) {
        map_worker_threads.push_back(thread(map_worker<k1,v1,k2,v2,v3>,
            inputFileName, map_fn, i, nr_mapper));
    }
    // Step 2: Wait for each thread workers to finish
    for (int i=0; i<nr_mapper; i++) {
        map_worker_threads[i].join();
    }
    return 0;
}

// Implementation of map_worker, must run in one thread.
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void map_worker(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1), 
        int thread_id, int nr_mapper) {
    int record_number = 0;
    vector<vector<pair<k2, v2>>> all_processed_records;
    ifstream file(inputFileName);
    string str;
    while (getline(file, str)) {
        if (hash_in_range<int>(record_number, nr_mapper) == thread_id) {
            vector<pair<k2,v2>> mapped_record = \
                map_fn(to_string(record_number),str);
            all_processed_records.push_back(mapped_record);
        }
        record_number += 1;
    }
    // Write the all_processed_records in a temp file
    std::ofstream fout("temp" + to_string(thread_id) + ".txt");
    for (auto record : all_processed_records) {
        for (auto elem : record) {
            fout << elem.first << " " << elem.second << " ";
        }
        fout << '\n';
    }
    fout.close();
}

template<typename k1, typename v1, typename k2, typename v2, typename v3>
int reduce_controller(string outputResultDirectory,
        vector<v3> (*reduce_fn)(k2, vector<v2>), int nr_reducer) {
    // Step 1: Spawn nr_reducer thread to do the reduction
    vector<thread> reduce_worker_threads;
    for (int i=0; i<nr_reducer; i++) {
        reduce_worker_threads.push_back(thread(reduce_worker<k1,v1,k2,v2,v3>,
            outputResultDirectory, reduce_fn, i, nr_reducer));
    }
    for (int i=0; i<nr_reducer; i++) {
        reduce_worker_threads[i].join();
    }
    return 0;
}

// Implementation of reduce_worker
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void reduce_worker(string outputResultDirectory, vector<v3> (*reduce_fn)(k2, vector<v2>), int thread_id, int nr_reducer) {
    vector<string> tempFiles;
    map<k2, vector<v2>> reducer_key_value_data;
    map<k2, vector<v2>> reducer_key_value_data_filtered;
    map<k2, vector<v3>> reduced_data;
    for (int i=0; i<nr_reducer; i++) {
        tempFiles.push_back("temp"+to_string(i)+".txt");
    }
    for (int i=0; i<nr_reducer; i++) {
        reducer_key_value_data = read_text<k2, v2>(tempFiles[i]);
        // Filter the data based on hash
        for (auto elem : reducer_key_value_data) {
            if (hash_in_range<k2>(elem.first, nr_reducer) != thread_id){
                continue;
            }
            // Extend the vector<v2> on k2 if already exists
            if (reducer_key_value_data_filtered.count(elem.first) == 0) {
                reducer_key_value_data_filtered.insert(pair<k2, vector<v2>>(elem.first, elem.second));
            } else {
                reducer_key_value_data_filtered[elem.first].insert(reducer_key_value_data_filtered[elem.first].end(), elem.second.begin(), elem.second.end());
            }
        }
    }
    for (auto elem : reducer_key_value_data_filtered) {
        reduced_data.insert(pair<k2, vector<v3>>(elem.first, reduce_fn(elem.first, elem.second)));
    }
    // Save the data into output.txt file in the specified output directory.
    write_map(outputResultDirectory + "_output"+to_string(thread_id)+".txt", reduced_data);
}
