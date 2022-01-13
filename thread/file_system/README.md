# Assignment 3

All declarations are in the `./include` folder and all implementations are in the `./src` folder.

To compile and run this project, you can run the `./compile` shell script (of course, you will need to `chmod +x ./compile.sh`) included in the base of this repo. This will generate
a Makefile, compile the code and run it. The output of this will be in the `./build` folder. Alternatively, just run the play
button in an IDE 😉️.

The structure of the super block was modified, but you can find its structure in the `include/helpers.h` file.

Some API declarations were modified, so some tests were modified as well.

To change which tester to use, you can comment/uncomment the right line in the `CMakeLists.txt` file.
All tester files are in the `./testers` folder. I had to change some lines in the tester 2 so that it would conform
with my implementation of the sfs API. It goes without saying that please use my testers for evaluating me (or modify 
yours with the following changes). Here are the changes:
- lines 15 and 381: I had a different number for the max filename length.
- line 64: I had a different number of maximum opened files.
- line 353 and 354: My implementation of sfs_open sets the read/write pointer to 0 so I just set it at the end of that file
so that the read test at the end won't fail when it reads the newly written information(a list of 0s).
- lines 383 and 424: My implementation of sfs_getnextfilename returns 0 on success, 1 if there are no more files, and -1
if there are just no files in the file system. I had to change the tester accordingly.

Note: You will need CMAKE for using the `./compile.sh` script. Compiling this project was intended with CMAKE version 3.16, but you can change to your version of cmake in the
cmake file.