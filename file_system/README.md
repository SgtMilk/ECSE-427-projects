# Assignment 3

This assignment's task was to code a one-level file system library that could be mounted with FUSE. You can find all the descriptions of the functions of the API in the `./include/sfs_api.h`.

## To run the first three testers

All declarations are in the `./include` folder and all implementations are in the `./src` folder.

To compile and run this project, you can run the `./compile` shell script (of course, you will need to `chmod +x ./compile.sh`) included in the base of this repo. This will generate
a Makefile, compile the code and run it. The output of this will be in the `./build` folder. 

To change which tester to use, you can comment/uncomment the right line in the `CMakeLists.txt` file. All tester files are in the `./testers` folder. 

Note: You will need CMAKE for using the `./compile.sh` script. Compiling this project was intended with CMAKE version 3.16, but you can change to your version of cmake in the
cmake file.

Note for test 2: Sometimes, it takes a bit of time to run, but is going to finish.