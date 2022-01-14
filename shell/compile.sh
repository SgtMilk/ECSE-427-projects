#!/bin/sh

mkdir build
cd build
cmake ../
make -j6
cd ..
./build/shell
