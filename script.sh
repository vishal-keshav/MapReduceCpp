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
cp ../extra/config_WordCounter.txt .
cp ../data/WordCounterInput.txt WordCounterData
echo "-------------------------------------------------------------------------------------------------------"
echo "Executing WordCounter program, the temporary and the outputs are generated in WordCounterData directory"
echo "-------------------------------------------------------------------------------------------------------"
./WordCounter

echo "---------------------------------------------------------------------"
echo "Copying the InvertedIndex input data into InvertedIndexData directory"
echo "---------------------------------------------------------------------"
mkdir InvertedIndexData
cp ../extra/config_InvertedIndex.txt .
cp ../data/InvertedIndexInput.txt InvertedIndexData
echo "-----------------------------------------------------------------------------------------------------------"
echo "Executing InvertedIndex program, the temporary and the outputs are generated in InvertedIndexData directory"
echo "-----------------------------------------------------------------------------------------------------------"
./InvertedIndex

echo "----------------------------------------------------------------------------"
echo "Copying the ReverseWeblinkGraph input data into ReverseWeblinkGraphData directory"
echo "----------------------------------------------------------------------------"
mkdir ReverseWeblinkGraphData
cp ../extra/config_ReverseWeblinkGraph.txt .
cp ../data/ReverseWeblinkGraphInput.txt ReverseWeblinkGraphData
echo "-----------------------------------------------------------------------------------------------------------------------"
echo "Executing ReverseWeblinkGraph program, the temporary and the outputs are generated in ReverseWeblinkGraphData directory"
echo "-----------------------------------------------------------------------------------------------------------------------"
./ReverseWeblinkGraph

echo "-------------------------------------------------------------------------------------------------"
echo "Now running the PySpark program to generate the expected output."
echo "Please make sure you have Python3 and PySpark installed in your system, else this test will fail"
echo "-------------------------------------------------------------------------------------------------"

cd ..
export PYSPARK_PYTHON=/usr/bin/python3
export PYSPARK_DRIVER_PYTHON=/usr/bin/ipython
python3 spark/WordCount.py
echo "----------------------------------------------------------------------------"
echo "PySpark output of WordCounter Application generated in build/WordCounterData"
echo "----------------------------------------------------------------------------"

python3 spark/ReverseWeblinkGraph.py
echo "--------------------------------------------------------------------------------------------"
echo "PySpark output of ReverseWeblinkGraph Application generated in build/ReverseWeblinkGraphData"
echo "--------------------------------------------------------------------------------------------"

python3 spark/InvertedIndex.py 
echo "--------------------------------------------------------------------------------"
echo "PySpark output of InvertedIndex Application generated in build/InvertedIndexData"
echo "--------------------------------------------------------------------------------"

echo "------------------"
echo "All tasks are done"
echo "------------------"

