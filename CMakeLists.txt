# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project(Mach7)

add_subdirectory(code/test)

# Define the library target
add_library(Mach7 INTERFACE)
add_library(Mach7::Mach7 ALIAS Mach7)

install(TARGETS Mach7
    EXPORT Mach7Targets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/Mach7Config.cmake.in"
    "${PROJECT_BINARY_DIR}/Mach7Config.cmake"
    INSTALL_DESTINATION lib/cmake/Mach7
)

install(EXPORT Mach7Targets DESTINATION lib/cmake/Mach7)
install(FILES
    "${PROJECT_BINARY_DIR}/Mach7Config.cmake"
    DESTINATION lib/cmake/Mach7)


install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/code/mach7/
    DESTINATION include/mach7)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/code/xtl/
    DESTINATION include/xtl)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/code/cppft/
    DESTINATION include/cppft)
