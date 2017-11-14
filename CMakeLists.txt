cmake_minimum_required(VERSION 3.8)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Main.cpp Commands.h Environment.h Files.h FileSystem.h GlobalVariables.h Files.cpp)
add_executable(untitled1 ${SOURCE_FILES})