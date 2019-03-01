# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

# Allow using solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add preprocessor definition to disable internal XTL messages
add_definitions(-DXTL_MESSAGE_ENABLED=0)

project(Tests)

add_subdirectory(unit)
add_subdirectory(time)


