#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "Utility.h"

// Declaration of map_worker
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void map_worker(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1));

// Declaration of reduce_worker
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void reduce_worker(string outputResultDirectory, vector<v3> (*reduce_fn)(k2, vector<v2>));

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
        // Spin up one worker thread for map task
        thread map_worker_thread(map_worker<k1,v1,k2,v2,v3>, 
                        this->inputFileName, this->map_fn);
        map_worker_thread.join();

        // Now create and start a thread for reduce task.
        thread reduce_worker_thread(reduce_worker<k1,v2,k2,v2,v3>,
                        this->outputResultDirectory, this->reduce_fn);
        reduce_worker_thread.join();
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

// Implementation of map_worker, must run in one thread.
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void map_worker(string inputFileName, vector<pair<k2, v2>> (*map_fn)(k1, v1)) {
    int record_number = 0;
    vector<vector<pair<k2, v2>>> all_processed_records;
    ifstream file(inputFileName);
    string str;
    while (getline(file, str)) {
        vector<pair<k2,v2>> mapped_record = \
            map_fn(to_string(record_number),str);
        all_processed_records.push_back(mapped_record);
        record_number += 1;
    }
    // Write the all_processed_records in a temp file
    std::ofstream fout("temp.txt");
    for (auto record : all_processed_records) {
        for (auto elem : record) {
            fout << elem.first << " " << elem.second << " ";
        }
        fout << '\n';
    }
    fout.close();
}

// Implementation of reduce_worker, must run in one thread.
template<typename k1, typename v1, typename k2, typename v2, typename v3>
void reduce_worker(string outputResultDirectory, vector<v3> (*reduce_fn)(k2, vector<v2>)) {
    vector<string> tempFiles{"temp.txt"};
    map<k2, vector<v2>> reducer_key_value_data = read_text<k2, v2>(tempFiles[0]);
    std::map<k2, vector<v3>> reduced_data;
    for (auto elem : reducer_key_value_data) {
        reduced_data.insert(pair<k2, vector<v3>>(elem.first, reduce_fn(elem.first, elem.second)));
    }
    // Save the data into output.txt file in the specified output directory.
    write_map(outputResultDirectory + "_output.txt", reduced_data);
}
