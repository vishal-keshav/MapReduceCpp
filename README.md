# MapReduceCpp
An implementation of multithreaded version of MapReduce that simulates the fault tolerance.

### Project Structure
`doc` Contains documentation related to the project, how to build the project and other pointers relevant to C++ programming.

`include` Contains header files

`src` Contains the source file for the library

`test` All code to test the modules

`lib` Extra libraries on which the project depends on

`extra` Contains source code to test new ideas and concepts.

`bin` Automatically generated on building the project. Contains the executable binaries.

`build` Automatically generated on building the project. Contains the intermediate compiled files.


## API specifications

### High-level Architecture
![architecture](extra/DesignDiagram/high_level_design.png "High-level design")

### Sample client program

```Python
# In the initial version of program, we only allow processing 
# a text file where each line is a record.
source_file_location = "/home/file/hamlet.txt"
output_directory = "/home/hamletout"
nr_mapper = 4
nr_reducer = 2

# specification of map user defined function
def udf_map(line_content):
    ret = []
    for word in line_content.split():
        ret.append((word, 1))
    return ret
# specification of reduce user defined function
def udf_reduce(word, count_list):
    ret = 0
    for count in count_list:
        ret += count
    return ret

# Create an instance of MapReduce by providing the specifications
mapReduce = MapReduceCpp(nr_mapper, nr_reducer)

# Execute the MapReduce processing (possible in a seperate thread)
sig = mapReduce.run(source_file_location, output_directory, udf_map, udf_reduce)
if sig == 'SUCCESS':
    return
else:
    print("Something went wrong")
```

### Master signature
Master lets client initialize an instance of itself and accepts number of mapper
and number of reducer in it's constructor.

Master exposes a public function `run` that accepts source_file_location
, output_directory and user defined function for map and reduce.

```C++
class Master {
    public:
        Master(int nr_mapper, int nr_reducer);
        int run(string source_file_name, string output_directory,
                (Iterator) map(String), String reduce(String, Iterator));
};
```

### Map Coordinator Signature
Map coordinator's main job is to take command from Master about executing the
map user defined function on nr_mapper threads and return the back the location
of temporary written files. Returns a MapperResult object once the task is done.
The MapperResult contains the success/failure along with the location in which
mapped partition are written.

Later, this module will extended to have fault tolerance.

```C++
class MapCoordinator {
    public:
        MapperResult run_mapper(string source_file_name, (Iterator) map(String),
                int nr_mapper);
};
```

### Reduce Coordinator Signature
Takes the temporary partition from master and starts the reduction job.

```C++
class ReduceCoordinator {
    public:
        ReducerResult run_reducer(string temporary_partitioned_files, 
            (int) reduce(int, Iterator), int nr_reducer);
};
```

### Worker Signature
A single unit of thread that execute an arbitrary function in one thread.

```C++
class Worker {
    public:
        int execute(function_pointer);
}
```

### Contributors:
Jessie Huo

Vishal Keshav

Kenneth Myers