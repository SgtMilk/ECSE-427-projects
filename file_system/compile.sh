#!/bin/sh

rm -rf ./testers/fuse_wrap_new.o
rm -rf ./testers/fuse_wrap_old.o
rm -rf ./src/disk_emu.o
rm -rf ./src/helpers.o
rm -rf ./src/linkedList.o
rm -rf ./src/sfs_api.o
rm -rf build
mkdir build
cd build || exit
cmake ../
make -j6
cd ..
./build/sfs
