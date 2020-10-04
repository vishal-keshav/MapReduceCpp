#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <utility>

#include "Mapper.hpp"
#include "MapperResult.hpp"

using namespace std;

MapperResult Mapper::execute(string inputFileName, 
        vector<pair<string, int>> (*map_fn)(string key, string value),
        int nr_mapper){
    // For now, ignore everythin and process the inputFileName
    // using map_fn here itself. Write the output file as temp.txt
    int record_number = 0;
    vector<vector<pair<string, int>>> all_processed_records;
    ifstream file("input.txt");
    string str;
    while (getline(file, str)) {
        vector<pair<string,int>> mapped_record = \
                map_fn(to_string(record_number),str);
        all_processed_records.push_back(mapped_record);
        record_number += 1;
    }
    // Write the all_processed_records in a temp file
    std::ofstream fout("temp.txt");
    for (auto record : all_processed_records) {
        for (auto elem : record) {
            fout << elem.first << " " << elem.second;
        }
        fout << '\n';
    }
    // Return the result signals to the caller
    vector<string> tempFiles{"temp.txt"};
    MapperResult res(0, tempFiles);
    return res;
}
