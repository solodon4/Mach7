cmake_minimum_required(VERSION 3.3)
include_directories(../..)

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
exp
exp2
expr
expr_meta
filter
memoized_cast
morton
non_unique_problem
non_unique_workaround
pattern_composition
predicate
prolog
prolog-pat
prolog-pat2
regex
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
  add_executable(${program} ${program}.cpp)
endforeach(program)

add_executable(syntax syntax.cxx)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED OFF)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 QUIET COMPONENTS any variant HINTS $ENV{BOOST}) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(xtl xtl.cpp) 
    target_link_libraries(progname ${Boost_LIBRARIES})
endif()
