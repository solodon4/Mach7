# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

# Allow using solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add preprocessor definition to disable internal XTL messages
add_definitions(-DXTL_MESSAGE_ENABLED=0)

# Sets the name of the project, and stores it in the variable PROJECT_NAME. When called from the top-level CMakeLists.txt also stores the project name in the variable CMAKE_PROJECT_NAME.
project(Tests)

# Adds a subdirectory to the build. The source_dir specifies the directory in which the source CMakeLists.txt and code files are located.
add_subdirectory(unit)
add_subdirectory(time)
