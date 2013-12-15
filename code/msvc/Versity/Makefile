#
# \file
#
# Makefile for building and testing Versity.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
# Copyright (C) 2011-2012 Texas A&M University.
# All rights reserved.
#

#
# Usage:
#     make        - Build all .cpp files in the current folder
#     make all    - Build all the targets and documentation
#     make clean  - Clean all targets
#     make doc    - Build Mach7 documentation
#     make includes.png - Build graph representation of header inclusions
#     make test   - Run the test suite
#

OS      = $(shell uname -s)
CXX     = g++
EXTRAFLAGS = -Wall -Wno-long-long  -ftemplate-depth-250

ifeq ($(OS),Darwin)
EXTRAFLAGS = $(EXTRAFLAGS) -Wno-long-double
endif

INCLUDES = -Imach7
LIBS     = -lstdc++
CXXFLAGS = $(INCLUDES) -Wall -time -O2 -DNDEBUG -std=gnu++0x $(EXTRAFLAGS)
LIBFLAGS = $(LIBS)
AR       = ar

.SUFFIXES: .cpp .o

# Create a list of source files.
SOURCES = optimize.cpp versity-types.cpp versity.cpp versity.tab.cpp versity.yy.cpp
# Create a list of object files from the source file lists.
OBJECTS = ${SOURCES:.cpp=.o}
# Create a list of targets.
TARGETS = versity.exe
# Create a list of dependency files.
DEP_FILES = $(SOURCES:.cpp=.dep)

# Setting .SECONDARY to blank disables deletion of intermediate files by make
#.SECONDARY:

#include $(DEP_FILES)

# A phony target is one that is not really the name of a file; rather it is just 
# a name for a recipe to be executed when you make an explicit request.

.PHONY: all clean cmp default doc syntax tags test timing ver

# Phony targets can have prerequisites. When one phony target is a prerequisite 
# of another, it serves as a subroutine of the other.

# Build all files in current folder
all: default 

# A phony target should not be a prerequisite of a real target file; if it is, 
# its recipe will be run every time make goes to update that file. 

# Build all targets by default
default: 	$(TARGETS)

# A rule to build .exe file out of a .o file
%.exe: $(OBJECTS)
	@echo "SOURCES=" $(SOURCES)
	@echo "OBJECTS=" $(OBJECTS)
	@echo "TARGETS=" $(TARGETS)
	@echo --------------------------------------------------------------------------------
	@echo Linking $@ ...
	$(CXX) -o $@ $(LIBFLAGS) $(OBJECTS) $(LIBS)
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

# A rule to clean all the intermediates and targets
clean:
	rm -rf $(TARGETS) $(OBJECTS) $(DEP_FILES) *.output *.tab.cpp *.tab.hpp *.yy.cpp includes.dot includes.png

# A rule to create GraphViz .dot file with graph representation of header inclusions
includes.dot:
	grep -R --include=*.hpp "#include" * | sed -e "s/^/\"/g" -e "s/:/\"/g" -e "s/\/\/.*//g" -e "s/\"[A-Za-z0-9_\.\/-]*\//\"/g" -e "s/\(<.*>\)/\1\n\1 [color=red,shape=box]/g" -e "s/#include/ -> /g" -e "1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }" -e "$$ { s/$$/\n}\n/g }" > includes.dot

# A rule to convert the above GraphViz .dot file into actual picture
includes.png: includes.dot
	dot.exe -Tpng -oincludes.png includes.dot

# A rule to run set of tests
test:
	@for file in *.exe; do \
	    name=`basename $$file .exe` ; \
	    name=`basename $$name .crash` ; \
	    if (ls $$name.crash.exe >/dev/null 2>&1) ; then \
	        continue ; \
	    fi ; \
	    echo ======================================== [ $$file ] ; \
	    if (ls $$name.*in >/dev/null 2>&1) ; then \
	        for f in $$name.*in; do \
	            echo ; \
	            echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ { "$$f" } ; \
	            cat "$$f" ; \
	            echo ; \
	            echo ---------------------------------------- ; \
	            ./$$file < "$$f" ; \
	        done ; \
	    else \
	      ./$$file; \
	    fi \
	done ; \
	echo -n 
