# MapReduceCpp
An implementation of distributed MapReduce that simulates the fault tolerance on a single machine.

## Contributors:
Jessie Huo

Vishal Keshav

Kenneth Myers

## For the purpose of project evaluation, please follow the step-by-step guidelines
### Requirements
* Since the the project is implemented in C++, you should have atleast C++17 installed on your system.
* Since the project uses CMake to build the project (the MapReduce library and three use-case programs), CMake >= 3.15 should be installed on your system.
### Script to automatically build and run sample programs.
`sh script.sh`

Running this command builds the `libMapReduceCpp.so` library, builds three sample program namely `WordCounter`, `InvertedIndex` and `ReverseWeblinkGaph` (and links the library to these programs). All the builds are placed in a directory `build` (will be created automatically). The output of the three programs are also placed in the `build` director (WordCounter_output.txt, InvertedIndex_output.txt and ReverseWeblinkGraph_output.txt). Please check the corresponding inputs to these program in the `data` directory.

## Project details

### Implementation
The MapReduceCpp is a header only library. The source code for the library can be found in `include` directory. Most importantly, `include/MapReduceMaster.h` is the entry point to use this library. This header file when included into a program, provided the access to an object `MapReduceMaster` and an API `process()` to start the processing. Refer below the pseudo program to understand the API.

```C++

#include "MapReduceMaster.h"

class MyAwesomeMapReduce : MapReduceInterface {
    void map_fn(string key, string value) {
        // Your map function implementation here
        // Dont forget to call emitIntermediate() on each key-value pair after mapping
    }

    void reduce_fn(string key, vector<string> value_list) {
        // Your reduce function implementation here
        // Dont forget to call emit() on each key-value_list pair after reduction
    }
}

// Now register your awesome map reduce implementation
MapReduceInterfaceFactoryRegistration<MyAwesomeMapReduce> _MyAwesomeMapReduce("MapReduce");

int main() {
    MapReduceMaster masterObj("inputFileName.txt", "dataDirectory", Number_of_worker);
    int status = masterObj.process();
    return 0;
}

```

Please check sample implementation of WordCounter in the file `src/WordCounter.cpp`, InvertedIndex in the file `src/invertedIndex.cpp` and ReverseWeblinkGraph in the file `src/ReverseWeblinkGraph.cpp` for more details.

### High-level design
A high level design of the library is shown below:
![architecture](extra/DesignDiagram/high_level_design.png "High-level design")

The interaction diagram is shown below:
![interaction](extra/DesignDiagram/version2_distributed.png "Interaction diagram")

The program uses N servers on different process. We ran the WordCounter program on 10 servers.
The `sudo netstat -ltnp` terminal output is shown in the below snapshot:
![server](extra/server_ports.png "Server listening on different ports on different process")

### Fault tolerance upto one server failure
To test the fault tolerance of map and reduce operations (upto one failure), we did the following:
1. Added pseudo work load (by adding sleep for 5 to 10 seconds) on the map and reduce workers.
2. Started the WordCounter Program on two servers.
3. Started tracking the process id for each ports (using command `netstat -nltp -c`)
4. Randomly killed (using `kill -9 PID`) a map process listening on one of the ports.
5. Randomly killed (using `kill -9 PID`) a reduce process listening on one of the ports.

The result was:
MapReduce library successfully recovered the failed server by detecting the server fail using heart-beat mechanism
and started a new server listening on the old port. Following this, the map reduce library also
called the map (or else reduce) task on the newly started server.

A snapshot of the experiment is shown below:
![server_failure](extra/server_fail_test.gif "Testing the fault-tolerance of the map-reduce library")

### Project Structure
`doc` Contains documentation related to the project, how to build the project and other pointers relevant to C++ programming.

`include` Contains header files of the MapReduceCpp library.

`src` Contains the source file of the programs that uses MapReduceCpp library.

`test` Contains unittests, uses googletest

`lib` Extra libraries on which the project depends on, these comes along with the package.

`extra` Contains source code to test new ideas, concepts and design.

`build` Automatically generated on building the project. Contains the intermediate compiled files and the compiled MapReduceCpp library.

`spark` Contains the respective Spark codes for the three applications to check the correctness of our MapReduce implementation. Output is written to /build/WordCount_SparkOutput.txt, /build/InvertedIndex_SparkOutput.txt, and /build/ReverseWeblinkGraph_SparkOutput.txt. Need Spark installed.

## Project Implementation

### Master
Master is implemented by MapReduceMaster class, which initializes with the input file name, output file name, map function pointer, reduce function pointer, and number of worker threads. On calling the process function, it creates the several workers for map, waits for each of the thread to return a success signal, then creates and lanches workers for reduce. Once all workers are returned, the master returns with a success signal.

The client program can invoke the process in a seperate thread without blocking, however, this is optional.

### Mapper
Mapper is a logical entity, which is implemented in the process() method of the MapReduceMaster class.

### Reducer
Reducer is a logical entity, which is implemented in the process() method of the MapReduceMaster class.

### Workers
Implemented in the MapReduceMaster file. This will be moved out later on.

