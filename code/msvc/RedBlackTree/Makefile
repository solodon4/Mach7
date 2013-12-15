#
# \file
#
# Makefile for building and testing RedBlackTree example.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
# Copyright (C) 2011-2012 Texas A&M University.
# All rights reserved.
#

TARGETS = red-black-tree.exe
SOURCES = $(shell ls *.cpp)
OS      = $(shell uname -s)
CXX     = g++
EXTRAFLAGS = -Wall -Wno-long-long  -ftemplate-depth-250

ifeq ($(OS),Darwin)
EXTRAFLAGS = $(EXTRAFLAGS) -Wno-long-double
endif

INCLUDES = -I../..
LIBS     = -lstdc++
CXXFLAGS = $(INCLUDES) -Wall -Wextra -pedantic -time -O2 -DNDEBUG -std=gnu++0x $(EXTRAFLAGS)
LIBFLAGS = $(LIBS)
AR       = ar

.SUFFIXES: .cpp .o

# Create a list of object files from the source file lists.
OBJECTS  = ${SOURCES:.cpp=.o}

DEP_FILES = $(SOURCES:.cpp=.dep)

# Setting .SECONDARY to blank disables deletion of intermediate files by make
.SECONDARY:

#include $(DEP_FILES)

# Generic targets

all: $(TARGETS)

# A rule to build .exe file out of a .o file
%.exe: $(OBJECTS)
	@echo --------------------------------------------------------------------------------
	@echo Linking $@ ...
	$(CXX) -o $@ *.o $(LIBFLAGS)
	@echo ================================================================================
	@echo Done building $@
	@echo ================================================================================
	@echo
	@echo

# A rule to build .o file out of a .cpp file
%.o: %.cpp
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

clean:
	rm -rf $(TARGETS) $(OBJECTS) $(DEP_FILES) *.output *.tab.cpp *.tab.hpp *.yy.cpp

# A rule to create GraphViz .dot file with graph representation of header inclusions
includes.dot:
	grep -R --include=*.hpp "#include" * | sed -e "s/^/\"/g" -e "s/:/\"/g" -e "s/\/\/.*//g" -e "s/\"[A-Za-z0-9_\.\/-]*\//\"/g" -e "s/\(<.*>\)/\1\n\1 [color=red,shape=box]/g" -e "s/#include/ -> /g" -e "1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }" -e "$$ { s/$$/\n}\n/g }" > includes.dot

# A rule to convert the above GraphViz .dot file into actual picture
includes.png: includes.dot
	dot.exe -Tpng -oincludes.png includes.dot
