#pragma once

class MapReduceResult {
public:
    MapReduceResult(int res):result(res){}
    int getResult(){
        return result;
    }
private:
    int result;
};
