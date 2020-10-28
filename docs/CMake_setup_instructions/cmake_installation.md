## Cmake installation

Skip this installation if `cmake --version` is greater than or equal to 3.15.

This project uses Cmake to build the library and the sample program. To install the latest version of CMake, follow the steps:

Step 1: Remove any older version of cmake in your system
`sudo apt remove --purge --auto-remove cmake`

Step 2: Download the cmake installation from [official website](https://cmake.org/download/)

Step 3: Go to the Download directory and execute followin commands to install
`cd /home/Downloads`
`tar -xzvf cmake-$version.$build.tar.gz`
`cd cmake-$version.$build/`
`./bootstrap`
`make -j$(nproc)`
`sudo make install`

Step 4: Now check your version
`cmake --version`