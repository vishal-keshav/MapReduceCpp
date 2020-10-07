mkdir build
cd build
cmake ..
cmake --build .
cp -r ../data/* .
./WordCounter
echo "Output is written in build/output.txt"