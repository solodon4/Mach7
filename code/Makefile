#
#  Mach7: Pattern Matching Library for C++
#
#  Copyright 2011-2013, Texas A&M University.
#  Copyright 2014 Yuriy Solodkyy.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#
#      * Neither the names of Mach7 project nor the names of its contributors
#        may be used to endorse or promote products derived from this software
#        without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# \file
#
# Makefile for building and testing pattern matching library.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# \see https://parasol.tamu.edu/mach7/
# \see https://parasol.tamu.edu/xtl/
# \see https://github.com/solodon4/Mach7
# \see https://github.com/solodon4/SELL
#

#
# Usage:
#     make        - Build all .cpp files in the current folder
#     make all    - Build all the targets and documentation
#     make unit   - Build all unit tests
#     make time   - Build all supported configurations for timing the library
#     make cmpl   - Build all supported configurations for timing the compilation times of the library
#     make clean  - Clean all targets
#     make doc    - Build Mach7 documentation
#     make includes.png - Build graph representation of header inclusions
#     make test   - Run the test suite
#     make check  - Run those tests for which there are correct_output/*.out files and check that the output is the same
#

GCC_COLORS=always
OS       = $(shell uname -s)
LIBS     = -lstdc++
INCLUDES =
# Our Travis CI machines install boost. Make variable non-empty to trigger compiling and testing boost tests.
ifeq (true,$(TRAVIS))
	BOOST_ROOT = .
endif
ifneq (,$(BOOST_ROOT))
	INCLUDES += -I$(BOOST_ROOT) -DHAS_BOOST
endif
CXXFLAGS = $(INCLUDES) -g -Wall -Wextra -pedantic -v -O2 -DNDEBUG -DXTL_MESSAGE_ENABLED=0 -std=c++0x
ifneq (,$(findstring g++,$(CXX)))
	CXXFLAGS += -Wno-vla -ftrack-macro-expansion=4 -fdiagnostics-show-option
endif
ifneq (,$(findstring clang,$(CXX)))
	CXXFLAGS += -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -Wno-vla-extension -Wno-nested-anon-types -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-anonymous-struct
endif

LIBFLAGS =
OCAMLLIB = /c/Program Files (x86)/OCaml/lib
#OCAMLLIB = C:\Program Files (x86)\OCaml\
FLEXLINKFLAGS=-L/mingw/lib -L/mingw/lib/gcc/mingw32/4.6.1
AR       = ar

.SUFFIXES: .cpp .o

# Create a list of source files.
SOURCES  = $(shell ls *.cpp)
# Create a list of object files from the source file lists.
OBJECTS  = ${SOURCES:.cpp=.o}
# Create a list of targets.
TARGETS  = ${SOURCES:.cpp=.exe}

# A phony target is one that is not really the name of a file; rather it is just
# a name for a recipe to be executed when you make an explicit request.

.PHONY: all clean cmp default doc syntax tags check test time unit cmpl ver

# Phony targets can have prerequisites. When one phony target is a prerequisite
# of another, it serves as a subroutine of the other.

# Build all tests
all: default

# A phony target should not be a prerequisite of a real target file; if it is,
# its recipe will be run every time make goes to update that file.

# Build all tests by default
default: unit time cmpl examples

# Setting .SECONDARY to blank disables deletion of intermediate files by make
#.SECONDARY:

#include $(DEP_FILES)

# Build documentation
doc: 	$(SOURCES)
	@(cd doxygen; doxygen mach7.dxg)

# Print compiler version
ver:
	@$(CXX) --version

# Files with extension .no-link.cpp are not intended for linking
%.no-link.exe: %.no-link.o
	@echo Linking skipped for $@
	@echo ================================================================================
	@echo Done building $@
	@echo ================================================================================
	@echo
	@echo

# A rule to build .exe file out of a .o file
%.exe: %.o
	@echo --------------------------------------------------------------------------------
	@echo Linking $@ ...
	$(CXX) -o $@ $(LIBFLAGS) $< $(LIBS)
	@echo ================================================================================
	@echo Done building $@
	@echo ================================================================================
	@echo
	@echo

# A rule to build .o file out of a .cpp file
%.o: %.cpp *.hpp patterns/*.hpp
	@echo --------------------------------------------------------------------------------
	@echo Compiling $@ ...
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.dep: %.cpp
	set -e; $(CXX) -M $(INCLUDES) -c $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.tab.cpp %.tab.hpp: %.y
	@echo --------------------------------------------------------------------------------
	@echo Creating parser $@ ...
	bison -d -v -o $@ $<

%.yy.cpp: %.l
	@echo --------------------------------------------------------------------------------
	@echo Creating lexer $@ ...
	flex -o$@ $<

#%.y: %.yyy
#	@echo --------------------------------------------------------------------------------
#	@echo Instrumenting parser $@ ...
#	sed -f ypp.sed $< > $@

# A rule to build all the compilation timing tests
cmpl:
	$(MAKE) -C test/compile-time CXX=$(CXX) BOOST_ROOT=$(BOOST_ROOT)

# A rule to build all the timing tests
time:
	$(MAKE) -C test/time CXX=$(CXX) BOOST_ROOT=$(BOOST_ROOT)

# A rule to build all the unit tests
unit:
	$(MAKE) -C test/unit CXX=$(CXX) BOOST_ROOT=$(BOOST_ROOT)

# A rule to build all the examples
examples:
	$(MAKE) -C test/examples/RedBlackTree CXX=$(CXX) BOOST_ROOT=$(BOOST_ROOT)
	$(MAKE) -C test/examples/Versity      CXX=$(CXX) BOOST_ROOT=$(BOOST_ROOT)

# A rule to clean all the intermediates and targets
clean:
	$(MAKE) -C test/unit clean
	$(MAKE) -C test/time clean
	$(MAKE) -C test/compile-time clean
	$(MAKE) -C test/examples/RedBlackTree clean
	$(MAKE) -C test/examples/Versity      clean
	rm -rf $(TARGETS) $(OBJECTS) *.out *.stackdump *.cmi *.cmx *.obj *.csv *.exe.dSYM time-*.exe syntax-*.exe cmp-*.exe cmp_*.exe cmp_*.hi cmp_*.o

# A rule to create GraphViz .dot file with graph representation of header inclusions
includes.dot:
	grep -R --include=*.?pp "#include" * | sed -e "s/^/\"/g" -e "s/:/\"/g" -e "s/\/\/.*//g" -e "s/\"[A-Za-z0-9_\.\/-]*\//\"/g" -e "s/\(<.*>\)/\1\n\1 [color=red,shape=box]/g" -e "s/#include/ -> /g" > includes.dot
# -e "1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }" -e "$$ { s/$$/\n}\n/g }"

# A rule to convert the above GraphViz .dot file into actual picture
includes.png: includes.dot
	dot -Tpng -oincludes.png includes.dot

# Run those examples for which there are correct_output/*.out files and check that output is the same
check:
	$(MAKE) -C test/unit check
	$(MAKE) -C test/time check

# A rule to run set of tests
test:
	$(MAKE) -C test/unit test
	$(MAKE) -C test/time test