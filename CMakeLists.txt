cmake_minimum_required(VERSION 3.20)
project(lilmap2 C)

set(CMAKE_C_STANDARD 11)

add_library(lilmap src/lilmap/lilmap.c src/lilmap/lilmap.h)

add_executable(lilmap-test src/main.c)

target_link_libraries(lilmap-test lilmap)
