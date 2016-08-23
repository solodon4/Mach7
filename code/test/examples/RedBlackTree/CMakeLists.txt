# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

# Uncomment this to list the name of all the features that can be required belov in needed_features
# message("Your CMake version supports these C++ features:")
# get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
# foreach(i ${known_features})
# message("${i}")
# endforeach()

# To access Mach7 library headers
include_directories(../../..)

project(RedBlackTree CXX)
add_executable(red-black-tree
red-black-tree.cpp
red-black-tree.hpp
)

set_property(TARGET red-black-tree PROPERTY FOLDER "PatMatImpl")

project(rbtree C)
add_executable(rbtree
rbtree.c
rbtree.h
testmain.c
)

set_property(TARGET rbtree PROPERTY FOLDER "RBTreeTest")

project(AnotherRBTree C)
add_executable(AnotherRBTree
AnotherRBTree.c
)

set_property(TARGET AnotherRBTree PROPERTY FOLDER "AnotherRBTree")
