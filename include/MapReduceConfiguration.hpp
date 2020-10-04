#pragma once

#include <string>
#include <vector>
#include <utility>

/*
 * A configuration with client creates and passes to the MapReduceMaster for
 * processing. Here, we have set mapper and reducer worker to default 1.
 */
class MapReduceConfiguration {
public:
    MapReduceConfiguration(
        std::string f_name,
        std::string out_dir,
        std::vector<std::pair<std::string, int>> (*m_fn)(
            std::string key, std::string value
        ),
        int (*r_fn)(std::string k, std::vector<int> values)
        ): inputFileName(f_name), outputResultDirectory(out_dir),
          map_fn(m_fn), reduce_fn(r_fn){}
private:
    std::string inputFileName;
    std::string outputResultDirectory;
    std::vector<std::pair<std::string, int>> (*map_fn)(
        std::string key, std::string value);
    int (*reduce_fn)(std::string k, std::vector<int> values);
    int nr_mapper{1};
    int nr_reducer{1};
};
