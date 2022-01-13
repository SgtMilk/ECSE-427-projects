cmake_minimum_required(VERSION 3.16)
project(sut C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS -pthread)

include_directories(sut PUBLIC "include/")
include_directories(sut PRIVATE "src/")

add_executable(sut src/main.c src/sut.c src/queue.c src/helpers.c)