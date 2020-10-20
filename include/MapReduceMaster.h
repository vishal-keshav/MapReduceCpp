/*
 * The header file contains the implmentation of MapReduceMaster and mapper/reducer
 * workers. The class accepts a generic map/reduce UDF function from the client
 * program. We support only one text file where each line in the text file is
 * considered to be on record. The text file can have any data type, like string,
 * int, float etc. Refer sample programs to understand the usage of APIs.
 * 
 * Implementation or the first deadline submission uses a single thread for mapper 
 * and a single thread for reducer.
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <map>
#include <utility>

#include "rpc/server.h"
#include "rpc/client.h"

#include "Utility.h"

/*
 * MapReduceInterface is the interface for clients to communicate how map and
 * reduce is done on the data.
 * MapReduceInterface is sub-classed by client where they implement two virtual
 * functions namely map_fn and reduce_fn. The APIs for map_fn and reduce_fn
 * is standardized. The signatature is explained in the function definition.
 */
class MapReduceInterface {
public:
    // The declaration is map_fn, should be implemented by child class.
    virtual void map_fn(string k1, string v1) = 0;
    // The declaration of reduce_fn, should be implemented by child class.
    virtual void reduce_fn(string k2, vector<string> v2_list) = 0;
protected:
    // This function is used by the client in their implementation of map_fn code.
    // Emits the intermediate key-value pairs found by map_fn logic.
    // The emitted values are stored in a private variable, which is used by 
    // map_controller_module to create the intermediate temp files.
    // These temp files are consumed by various reduce workers later on.
    // The temp files are written in dataDirectory.
    void emitIntermediate(string k2, string v2) {
        emitted_intermediates.push_back(make_pair(k2, v2));
    }

    // This function is used by the client in their implementation of reduce_fn code.
    // Emits the key-list_of_values found by reduce_fn logic.
    // The emitted values are stored in a private variable, which is used by
    // reduce_controller_module to create the final output files.
    // These files are written in the dataDirectory.
    void emit(string k2, vector<string> v3_list) {
        emitted_outputs.push_back(make_pair(k2, v3_list));
    }
public:
    vector<pair<string, string>> emitted_intermediates;
    vector<pair<string, vector<string>>> emitted_outputs;
};

// Forward declaration of map_controller_module.
int map_controller_module(string inputFileName, string dataDirectory, MapReduceInterface *map_reduce_fn, int nr_mapper, int nr_reducer, int mapper_id);
// Forward declaration of reduce_controller_module.
int reduce_controller_module(string dataDirectory, MapReduceInterface *map_reduce_fn, int nr_reducer, int nr_mapper, int reducer_id);

/*
 * MapReducerMaster process the request from the MapReduce client program.
 * The request to process (which is map-reduce) is sent to the process function as a parameter.
 * The parameter of the process (which represents a request by client program) contains:
 * 1. What is the map function which needs to be applied on the input data.
 * 2. What is the reduce function which needs to be applied on temporary produced mapped data.
 * This request is implemented by the client by implementing the MapReduceInterface class.
 * 
 * When client creates a MapReduceMaster object to start the process on requested map_fn and reduce_fn,
 * the MapReducerMaster already has information about:
 * 1. How many map worker are required.
 * 2. How many reduce worker are required.
 * 3. Where is the dataDirectory
 * 4. What is the input file name (should be located in dataDirectory)
 * 
 * MapReduceMaster is the only way a client program interacts with MapReduce library.
 * process() is the only call that a client program needs to make to start the map and reduce.
 * 
 * Once, the process is complete, the function returns with a success(0) signal to the client program.
 */
class MapReduceMaster {
public:
    // CTOR: takes input file name and the directory where the file is located.
    // TODO: Extend the contructor to take the nr_mapper and nr_reducer.
    MapReduceMaster(string f_name, string out_dir): inputFileName(f_name), outputResultDirectory(out_dir){}

    // The main function that the client calls to start the processing.
    // This function handles the creation of nr_reducer == nr_mapper process on nr_reducer == nr_mapper servers.
    // All the worker listens to the "map" and "reduce" commands. They are bounded on a port (decided by us, the creator os this library).
    int process(MapReduceInterface *map_reduce_fn) {
        // Start a server at port 8012
        rpc::server server(8012);
        server.bind("map", [&](void) {
            map_controller_module(this->inputFileName, this->outputResultDirectory, map_reduce_fn, this->nr_mapper, this->nr_reducer, 0);
            return;
        });
        server.bind("reduce", [&](void) {
            reduce_controller_module(this->outputResultDirectory, map_reduce_fn, this->nr_reducer, this->nr_mapper, 0);
            return;
        });
        server.async_run(1);
        // Now instruct the map server to start mapping
        rpc::client client("127.0.0.1", 8012);
        client.call("map");

        //Now ask server for reduction
        client.call("reduce");

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

/*
 * Map controller module signifies one unit of a worker that does the map task
 * This map controller module is supposed to be running in a process on a server
 * This process is started by the MapReduceMaster on a specific port.
 * 
 * Everything is read and written to dataDirectory (passed as an argument). See the APIs.
 * 
 * Once MapReduceMaster asks the server to do the mapping (by calling map_controller_module),
 * the server listening to this port with use the map_fn present in the map_reduce_fn object pointer.
 * The map_controller_module logic is described in the below steps:
 * 1. inputFileName file is sequentially read.
 * 2. A hash function on the line number is used to know which line this map worker needs to work on.
 * 3. The selected lines are processed with map_fn. All the key-value pair emitted by map_fn 
 *    are stored in the MapReduceInterface object (map_reduce_fn) itself.
 *    See: MapReduceInterface->emitted_intermediates.
 * 4. Then hash function is applied on each key (of the emitted key-value pairs)
 *    and a temp_m_r.txt is saved. Here, m denotes the m'th mapper who is writting
 *    the file and r denotes the r'th reducer who has to read this file.
 * 5. Once all files are written (all temp_m_r.txt files), the server doing the
 *    map job returns a success signal back to the master (who is acting as a client).
 */

int map_controller_module(string inputFileName, string dataDirectory, MapReduceInterface *map_reduce_fn, int nr_mapper, int nr_reducer, int mapper_id) {
    int record_number = 0;
    ifstream file(dataDirectory + "/" + inputFileName);
    string str;
    // Iterate over the input file, apply map function to the line which hashes to mapper_id (this mapper).
    // The map_fn called emit_intermediate which keeps on storing the key-value pair in a vector.
    while (getline(file, str)) {
        if (hash_in_range(record_number, nr_mapper) == mapper_id) {
            map_reduce_fn->map_fn(to_string(record_number),str);
        }
        record_number += 1;
    }
    // Create nr_reducer temp files. Each file will have a name that looks like this: "temp_[mapper_id]_[reducer_id].txt"
    // Here, [var] translates to value the variable val is storing. So, no square brackets in the temp file.
    // reducer_id for eack key-value is derived by hash(key, nr_reducer).
    vector<ofstream> temp_files;
    for (int reducer_id = 0; reducer_id < nr_reducer; reducer_id++) {
        temp_files.push_back(ofstream(dataDirectory + "/temp_"+to_string(mapper_id)+"_"+to_string(reducer_id)+".txt"));
    }
    // Now we iterate through each emitted intermediate key-value pair and write that
    // into temp_[mapper_id]_[reducer_idx].txt file, where reducer_idx = hash(key, nr_reducer).
    // Write intermediate key-value pairs to temp file.
    int reducer_idx;
    for (auto elem : map_reduce_fn->emitted_intermediates) {
        reducer_idx = hash_in_range(elem.first, nr_reducer);
        temp_files[reducer_idx] << elem.first << " " << elem.second << '\n';
    }
    // Close all the file
    for (int reducer_id = 0; reducer_id < nr_reducer; reducer_id++) {
        temp_files[reducer_id].close();
    }
    // Returning zero means everything is successfully done.
    return 0;
}

/*
 * Reduce controller module signifies one unit of reduce worker the does the reduction
 * task on all the key's that hashes to reducer_id (this reducer). MapReduceMaster
 * creates a process on the server (one worker) that listens to a port and start
 * the reduce job (by executing the code in reduce_controller_module) upon signaling from the master.
 * 
 * All data is read from and written to dataDirectory.
 * 
 * The reduce_controller_module main logic is described in the below steps:
 * 1. Reads all the files with the name temp_[mapper_id]_[reducer_id] where mapper_id ranges from 0 to nr_mapper-1.
 *    Some files can be blank, and that Okay.
 * 2. While reading each key-value from every file, it creates a vector of key-list_of_values. 
 *    This is done by the emit() function in the reduce_fn inside MapReduceInterface object.
 * 3. After the vector is fully formed, each key-list_of_values are written to output_[reducer_id].txt 
 */
int reduce_controller_module(string dataDirectory, MapReduceInterface *map_reduce_fn, int nr_reducer, int nr_mapper, int reducer_id) {
    string tempFile;
    // Look for each temp file created by all mapper for this reducer to consume,
    // and process (or reduce the data) in those files.
    // Each text file will be read in temporary key_value_temp variable.
    // All possible key_value from each temp files are collected in key_value_accumulated variable.
    map<string, vector<string>> key_value_temp;
    map<string, vector<string>> key_value_acc;
    for (int mapper_idx = 0; mapper_idx < nr_mapper; mapper_idx++) {
        tempFile = dataDirectory + "/temp_" + to_string(mapper_idx) + "_" + to_string(reducer_id) + ".txt";
        key_value_temp = read_text<string, string>(tempFile);
        // Accumulated this data in key_value_accumulated by extending the value vector of key_value_accumulated
        for (auto elem : key_value_temp) {
            // If this key already existed in the key_value_accumulated, then extend the values
            if (key_value_temp.count(elem.first) != 0) {
                key_value_acc[elem.first].insert(key_value_acc[elem.first].end(), elem.second.begin(), elem.second.end());
            } else { // Create an entry in key_value_accumulated
                key_value_acc.insert(pair<string, vector<string>>(elem.first, elem.second));
            }
        }
    }
    // Now, apply reduction, the key-list_of_values will be emitted in the vector
    for (auto elem : key_value_acc) {
        map_reduce_fn->reduce_fn(elem.first, elem.second);
    }
    // Write the data to output text file in the dataDirectory
    write_key_val_vector(dataDirectory + "/output.txt", map_reduce_fn->emitted_outputs);
    // return the success signal from this worker
    return 0;
}
