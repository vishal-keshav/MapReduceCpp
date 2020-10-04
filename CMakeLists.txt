cmake_minimum_required(VERSION 3.0.0)
project(MapReduce VERSION 0.1.0)

# This is the main library
# Add more headers and sourcefile in new lines as new files are added.

add_library(
    MapReduceCpp SHARED
    include/HelloWorld.hpp
    include/MapReduceConfiguration.hpp
    include/MapReduceResult.hpp
    include/MapReduceMaster.hpp
    include/MapperResult.hpp
    include/Mapper.hpp
    include/ReducerResult.hpp
    include/Reducer.hpp
    src/HelloWorld.cpp
    )

# This is gtest Unittest that links to the above library
# Add more test files in add_executable as needed.

add_subdirectory(lib/googletest)
add_executable(UnitTests
    test/UnitTest1.cpp)
target_link_libraries(UnitTests PUBLIC gtest_main MapReduceCpp)
add_test(
    Name UnitTests
    COMMAND UnitTests
)


# This is a sample app that uses the MapReduce library and
# runs a simple program. No need to change this.

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(TestApp TestApp.cpp)
target_link_libraries(TestApp PUBLIC MapReduceCpp)