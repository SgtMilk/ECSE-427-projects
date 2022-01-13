cmake_minimum_required(VERSION 3.16)
project(assignment1 C)

set(CMAKE_C_STANDARD 11)

add_executable(assignment1 src/main.c src/commands.c src/helpers.c)
target_include_directories(assignment1 PRIVATE "src/")
target_include_directories(assignment1 PUBLIC "include/")
