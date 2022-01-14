#!/bin/sh

mkdir build
cd build || exit
cmake ../
make -j6
cd ..
./build/sut
