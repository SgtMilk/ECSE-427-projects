# Assignment 2

All declarations are in the `./include` folder and all implementations are in the `./src` folder.

To compile this project, you can run the `./compile` shell script (of course, you will need to `chmod +x ./compile.sh`) included in the base of this repo. This will generate
a Makefile and compile the code. The output of this will be in the `./build` folder. Alternatively, just run the play
button in an IDE 😉️.

To change the number of C-EXEC threads, change the boolean at line 54 of `./src/sut.c`.

Note: Compiling this project was intended with cmake version 3.16, but you can change to your version of cmake in the
cmake file.