cmake_minimum_required(VERSION 3.0.0)

project(MapReduce VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")

add_subdirectory(lib/rpclib)
add_subdirectory(lib/googletest)

add_library(
    MapReduceCpp SHARED
    include/Utility.h
    include/MapReduceMaster.h
    src/Temp.cpp
    )
# target_link_libraries(MapReduceCpp PUBLIC rpc pthread)

add_executable(UnitTests
    test/UnitTest1.cpp)
target_link_libraries(UnitTests PUBLIC gtest_main MapReduceCpp rpc pthread)
add_test(
    Name UnitTests
    COMMAND UnitTests
)


include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(WordCounter src/WordCounter.cpp)
target_link_libraries(WordCounter PUBLIC MapReduceCpp rpc pthread)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(InvertedIndex src/InvertedIndex.cpp)
target_link_libraries(InvertedIndex PUBLIC MapReduceCpp rpc pthread)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(ReverseWeblinkGraph src/ReverseWeblinkGraph.cpp)
target_link_libraries(ReverseWeblinkGraph PUBLIC MapReduceCpp rpc pthread)