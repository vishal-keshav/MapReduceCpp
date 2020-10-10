#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility>

#include "MapReduceMaster.h"

using namespace std;

void map_fn(void) {
    //[TODO]
    // pass the file that lists the scraped website source-targets
    // map function accesses each line, splits on comma,
    // returns <target,source>

}

void reduce_fn(void) {
    //[TODO]
    // for a given key (target) this will just create a 
    // list of all of the sources that link to that target

}

int main() {
    // [TODO]
}