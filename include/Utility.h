#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

template <typename T1, typename T2>
map<T1, vector<T2>> read_text(string inputTextFile) {
    map<T1, vector<T2>> data;
    ifstream file_temp(inputTextFile);
    string line;
    T1 tempKey;
    T2 tempValue;
    while(!file_temp.eof()) {
	    getline(file_temp, line);
        if (line.size() == 0) {
            continue;
        }
	    istringstream iss(line);
        do {
            iss >> tempKey;
            iss >> tempValue;
            if (data.count(tempKey) == 0) {
                vector<T2> tempValues{tempValue};
                data.insert(pair<T1, vector<T2>>(tempKey, tempValues));
            } else {
                data[tempKey].push_back(tempValue);
            }
        } while (iss);
    }
    return data;
}


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
