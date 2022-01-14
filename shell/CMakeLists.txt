cmake_minimum_required(VERSION 3.16)
project(shell C)

set(CMAKE_C_STANDARD 11)

include_directories(shell PUBLIC "include/")
include_directories(shell PRIVATE "src/")

add_executable(shell src/main.c src/commands.c src/helpers.c)