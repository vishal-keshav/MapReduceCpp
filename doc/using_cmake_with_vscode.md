# Using CMake with VSCode

I have assumed that the project is configured to use CMake, like Kit has be selected already.

VSCode provides a CMake Tools extention. Install this extention.
Then a new icon for CMake appear on the side bar. Click on that.
The CMake project outline will appear, which will have three dots on top.
In that, click on Clean Reconfigure All Projects. Once done, you dont need to do this again.
Now, in the MapReduce project, targets such as UnitTests, MapReduceCpp and TestApp can be build.
To run the UnitTest executable after you build, click play button at the botton status bar. The tests will run.