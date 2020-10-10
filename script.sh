mkdir build
cd build
cmake ..
cmake --build .
cp -r ../data/* .
./WordCounter
echo "Output is written in build/WordCounter_output.txt"
./InvertedIndex
echo "Output is written in build/InvertedIndex_output.txt"