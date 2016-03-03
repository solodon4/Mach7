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
exception_select_random
generic_select_kind
generic_select_random
hierarchy2
hierarchy2a
hierarchy2b
lambda
lambda-vir
numbers
numbers-new
ocaml_cmp
ocaml_cmp_kind
shape2
shape3
synthetic
synthetic_dynamic_cast
synthetic_dynamic_cast_binary
synthetic_dynamic_cast_cohen
synthetic_dynamic_cast_fast
synthetic_dynamic_cast_switch
synthetic_select
synthetic_select_kind
synthetic_select_random
synthetic_select1
synthetic_select2
synthetic_select3
synthetic_select4
time-pat-factorial0
time-pat-factorial1
time-pat-factorial2
time-pat-fibonacci
time-pat-gcd1
time-pat-gcd2
time-pat-gcd3
time-pat-power
time-vir-factorial0
time-vir-factorial1
time-vir-factorial2
time-vir-factorial3
time-vir-factorial4
time-vir-fibonacci
time-vir-gcd1
time-vir-gcd2
time-vir-gcd3
time-vir-power
time_type_switch1
time_type_switch2
time_type_switch3
time_type_switch4
type_switch
virpat
virpat0
virpat1
virpat2
virpat3-pat
virpat3-vir
)

foreach(program ${PROGRAMS})
  project(${program} CXX)
  add_executable(${program} ${program}.cpp)
  target_compile_features(${program} PRIVATE ${needed_features})
  set_property(TARGET ${program} PROPERTY FOLDER "Tests/Time")
endforeach(program)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED OFF)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 QUIET COMPONENTS any variant HINTS $ENV{BOOST_ROOT}) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(time-xtl-any time-xtl-any.cpp) 
    add_executable(time-xtl-variant time-xtl-variant.cpp)
    target_link_libraries(progname ${Boost_LIBRARIES})
endif()
