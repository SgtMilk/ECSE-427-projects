cmake_minimum_required(VERSION 3.16)
project(sfs C)

set(CMAKE_C_STANDARD 11)

include_directories(sfs PUBLIC "include/")
include_directories(sfs PRIVATE "src/")
include_directories(sfs PRIVATE "testers/")

set(SOURCES src/sfs_api.c src/helpers.c src/disk_emu.c src/linkedList.c)

# Uncomment on of the following five lines to compile
#add_executable(sfs ${SOURCES} testers/sfs_test0.c)
#add_executable(sfs ${SOURCES} testers/sfs_test1.c)
add_executable(sfs ${SOURCES} testers/sfs_test2.c)
#add_executable(sfs ${SOURCES} testers/fuse_wrap_old.c)
#add_executable(sfs ${SOURCES} testers/fuse_wrap_new.c)