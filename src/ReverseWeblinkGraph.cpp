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

// There should be a function that maybe runs separately first in main
// This function will check if we already have html data for top 500 websites
// if not go to the top 500 websites and scrape their html, 
// save it to a separate sub-folder in data folder, give it just the website name
// (not sure if this will get screwed up from file extensions)
// In the actual data folder save a new file that lists the file names 
// for websites we were able to scrape

void map_fn(void) {
    //[TODO]
    // pass the file that lists the scraped website datafiles
    // map function accesses each of these files and looks for
    // regular expressions of websites names (clip to just domain name)
    // ignore those that match the current domain
    // output <target, source> for each one

}

void reduce_fn(void) {
    //[TODO]
    // for a given key (target) this will just create a 
    // list of all of the sources that link to that target
}

int main() {
    // [TODO]
}