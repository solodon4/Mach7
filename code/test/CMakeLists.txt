# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project(Tests)

add_subdirectory(unit)
add_subdirectory(time)
