cmake_minimum_required(VERSION 3.8)
project(SPL-ass1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Main.cpp Commands.h Environment.h Files.h FileSystem.h GlobalVariables.h Files.cpp Environment.cpp FileSystem.cpp Commands.cpp)
add_executable(SPL-ass1 ${SOURCE_FILES})