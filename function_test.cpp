#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <fstream>
#include <sstream>

using namespace std;

vector<pair<string, int>> func1(int k, string value) {
    cout << "f1 invoked" << endl;
    vector<pair<string, int>> ret{make_pair(value, k)};
    return ret;
}

vector<pair<float, float>> func2(int k, string value) {
    cout << "f2 invoked" << endl;
    vector<pair<float, float>> ret{make_pair(0.1, 0.2)};
    return ret;
}

vector<int> func3(string k, vector<int> values) {
    vector<int> ret{100};
    cout << "f3 invoked" << endl;
    return ret;
}

vector<string> func4(float, vector<float> values) {
    vector<string> ret{"200"};
    cout << "f4 invoked" << endl;
    return ret;
}

template<typename k1, typename v1, typename k2, typename v2, typename v3>
void doFunction( vector<pair<k2, v2>> (*f1)(k1, v1), string d, vector<v3> (*f2)(k2, vector<v2>)) {
    // suppose only one record as d has come.
    vector<pair<k2, v2>> ret = f1(0, d);
    for (auto elem : ret) {
        cout << elem.first << " " << elem.second << endl;
    }
    vector<v2> values{ret[0].second};
    k2 key = ret[0].first;
    vector<v3> out = f2(key, values);
    for (auto e : out) {
        cout << e << endl;
    }
    cout << "function pointer checked" << endl;
}

int main() {
    doFunction(func1, "a word", func3);
    doFunction(func2 , "100 1000", func4);
    return 0;    
}

