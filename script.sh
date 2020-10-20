#!/bin/bash

# Build the library and the programs in build directory
echo "------------------------------------------------------------------------------"
echo "Building the MapReduceCpp library and three sample programs in build directory"
echo "------------------------------------------------------------------------------"
mkdir build
cd build
cmake ..
cmake --build .

# Create the directory for each program and copy the input data into those directory.
echo "-----------------------------------------------------------------"
echo "Copying the WordCounter input data into WordCounterData directory"
echo "-----------------------------------------------------------------"
mkdir WordCounterData
cp ../data/WordCounterInput.txt WordCounterData
echo "-------------------------------------------------------------------------------------------------------"
echo "Executing WordCounter program, the temporary and the outputs are generated in WordCounterData directory"
echo "-------------------------------------------------------------------------------------------------------"
./WordCounter

echo "---------------------------------------------------------------------"
echo "Copying the InvertedIndex input data into InvertedIndexData directory"
echo "---------------------------------------------------------------------"
mkdir InvertedIndexData
cp ../data/InvertedIndexInput.txt InvertedIndexData
echo "-----------------------------------------------------------------------------------------------------------"
echo "Executing InvertedIndex program, the temporary and the outputs are generated in InvertedIndexData directory"
echo "-----------------------------------------------------------------------------------------------------------"
./InvertedIndex

echo "----------------------------------------------------------------------------"
echo "Copying the ReverseWeblinkGraph input data into ReverseWeblinkGraphData directory"
echo "----------------------------------------------------------------------------"
mkdir ReverseWeblinkGraphData
cp ../data/ReverseWeblinkGraphInput.txt ReverseWeblinkGraphData
echo "-----------------------------------------------------------------------------------------------------------------------"
echo "Executing ReverseWeblinkGraph program, the temporary and the outputs are generated in ReverseWeblinkGraphData directory"
echo "-----------------------------------------------------------------------------------------------------------------------"
./ReverseWeblinkGraph

echo "------------------"
echo "All tasks are done"
echo "------------------"