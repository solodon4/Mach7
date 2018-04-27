# Version 3.2 is needed to be able to have support of target_compile_features for AppleClang
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

# Uncomment this to list the name of all the features that can be required belov in needed_features
# message("Your CMake version supports these C++ features:")
# get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
# foreach(i ${known_features})
# message("${i}")
# endforeach()

# To access Mach7 library headers
include_directories(../..)

# CMake would infer the options to pass to the compiler to ensure these features are supported (e.g. proper C++ version)
set(needed_features
cxx_auto_type)

# these are all compiled the same way
set(PROGRAMS
algebraic
category
cppcon-matching
cppcon-visitors
example01
example02
example03
example04
example04-smart-ptr
example04-smart-ptr-bind-ptr
example05
example06-smart-ptr
exp
exp2
expr
expr_meta
extractor
filter
guards
memoized_cast
morton
non_unique_problem
non_unique_workaround
one_of
pattern_composition
predicate
prolog
prolog-pat
prolog-pat2
regex
shape
shape4
shape5
shape6
shape7
shape8
type_switch2
type_switch3
type_switchN
type_switchN-decl
type_switchN-patterns
virpat-shapes
)

foreach(program ${PROGRAMS})
  project(${program} CXX)
  add_executable(${program} ${program}.cpp)
  target_compile_features(${program} PRIVATE ${needed_features})
  set_property(TARGET ${program} PROPERTY FOLDER "Tests/Unit")
endforeach(program)

project(syntax CXX)
add_executable(syntax syntax.cxx)
target_compile_features(syntax PRIVATE ${needed_features})
set_property(TARGET syntax PROPERTY FOLDER "Tests/Unit")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED OFF)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 QUIET COMPONENTS any variant HINTS $ENV{BOOST_ROOT}) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(xtl xtl.cpp) 
    target_link_libraries(progname ${Boost_LIBRARIES})
endif()
