
                        PATTERN MATCHING LIBRARY FOR C++
                               by Yuriy Solodkyy 
                           <yuriy.solodkyy@gmail.com>

Building sources
================

Using GCC (4.4 or later)
------------------------
 * make
    - builds .exe files from all the .cpp files in current directory.
 * make timings
    - builds all combinations of encodings, syntax and benchmarks out of 
      skeleton.cxx for timing purposes
 * make syntax
    - builds all combinations of configuration flags supported by the library to 
      make sure nothing was omitted
 * make test
    - runs all the .exe files in the current folder

Using Visual C++ (2010 or later)
--------------------------------
 * test-pm-timing.bat
    - builds all combinations of encodings, syntax and benchmarks out of 
      skeleton.cxx for timing purposes (same as "make timings" for Visual C++)
 * test-pgo.bat
    - compiles and performs profile-guided optimizations on all files passed as 
      arguments
 * test-pm.bat
    - builds sources varying amount of derived classes and virtual functions in 
      them
 * test-pm-daily.bat
    - builds all files in the test suite
 * test-pm-daily-pgo.bat
    - builds all files in the test suite and performs profile-guided 
      optimizations on them
 * ttt.bat
    - converts the summary of outputs into a latex definitions used in the 
      performance table

Known bugs and limitations
==========================

The following files crash GCC 4.4.5 on my Fedora 13 box. If they do on yours 
too, just delete them, they are all test cases anyways.
 - extractor.cpp
 - shape2.cpp
 - shape4.cpp
 - shape5.cpp
 - shape6.cpp
 - shape.cpp
 - numbers.cpp
 - category.cpp
 - exp.cpp

This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
Copyright (C) 2005-2011 Texas A&M University. All rights reserved.
