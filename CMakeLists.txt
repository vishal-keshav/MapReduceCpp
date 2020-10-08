cmake_minimum_required(VERSION 3.0.0)

project(MapReduce VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")

add_library(
    MapReduceCpp SHARED
    include/Utility.h
    include/MapReduceMaster.h
    src/Temp.cpp
    )


add_subdirectory(lib/googletest)
add_executable(UnitTests
    test/UnitTest1.cpp)
target_link_libraries(UnitTests PUBLIC gtest_main MapReduceCpp)
add_test(
    Name UnitTests
    COMMAND UnitTests
)


include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(WordCounter src/WordCounter.cpp)
target_link_libraries(WordCounter PUBLIC MapReduceCpp pthread)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(InvertedIndex src/InvertedIndex.cpp)
target_link_libraries(InvertedIndex PUBLIC MapReduceCpp pthread)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(ReverseWeblinkGraph src/ReverseWeblinkGraph.cpp)
target_link_libraries(ReverseWeblinkGraph PUBLIC MapReduceCpp pthread)