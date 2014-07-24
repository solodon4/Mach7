                                    Mach7
                       A PATTERN MATCHING LIBRARY FOR C++
                               by Yuriy Solodkyy 
                           <yuriy.solodkyy@gmail.com>


License Agreement (BSD)
=======================

Mach7: Pattern Matching Library for C++

Copyright 2011-2013, Texas A&M University.
Copyright 2014 Yuriy Solodkyy.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the names of Mach7 project nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Building sources
================

Using GCC (4.4 or later) or Clang (5.1 or later)
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
 - shape6.cpp7
 - shape.cpp
 - numbers.cpp
 - category.cpp
 - exp.cpp

Support
=======

Please contact Yuriy Solodkyy at yuriy.solodkyy@gmail.com with any questions regarding Mach7.
