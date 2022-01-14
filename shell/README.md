# Assignment 1

This assignment's task was to code a shell program with integrated (`pwd`, `cd`, etc...) and non-integrated programs (`ls`, `ping`, etc...), as well as with piping.

## To run 

All declarations are in the `./include` folder and all implementations are in the `./src` folder.

To compile and run this project, you can run the `./compile` shell script (of course, you will need to `chmod +x ./compile.sh`) included in the base of this repo. This will generate
a Makefile, compile the code and run it. The output of this will be in the `./build` folder. 

Note: You will need CMAKE for using the `./compile.sh` script. Compiling this project was intended with CMAKE version 3.16, but you can change to your version of cmake in the
cmake file.