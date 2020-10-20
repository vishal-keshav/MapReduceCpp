#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

// reads the output files of mapper and aggregate values of the same key
template <typename T1, typename T2>
map<T1, vector<T2>> read_text(string inputTextFile) {
    map<T1, vector<T2>> data;
    ifstream file_temp(inputTextFile);
    T1 tempKey;
    T2 tempValue;
    // Loop over the intermediate key-value pairs, push each value to an array
    // that is mapped to each key
    while (file_temp >> tempKey && file_temp >> tempValue){
        if (tempKey.size() == 0) {
            continue;
        }
        // if key not in the mapping yet, create a pair of this key value and
        // add to mapping, else push to existing array
        if (data.count(tempKey) == 0) {
            vector<T2> tempValues{tempValue};
            data.insert(pair<T1, vector<T2>>(tempKey, tempValues));
        } else {
            data[tempKey].push_back(tempValue);
        }
    }
    return data;
}

// Takes an output file and the reduced data. Writes the data to the file in
// format "key value1 [value 2 ...]"
template <typename T1, typename T2>
void write_map(std::string outputFileName, std::map<T1, std::vector<T2>> data) {
    ofstream fout(outputFileName);
    for (auto elem : data) {
        fout << elem.first << " ";
        for (auto v : elem.second) {
            fout << v << " ";
        }
        fout << '\n';
    }
}

void write_key_val_vector(std::string outputFileName,
                          std::vector<std::pair<std::string,
                          std::vector<std::string>>> data) {
    ofstream fout(outputFileName);
    for (auto elem : data) {
        fout << elem.first << " ";
        for (auto v : elem.second) {
            fout << v << " ";
        }
        fout << '\n';
    }
}
