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
# Makefile for building and testing RedBlackTree example.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# \see https://parasol.tamu.edu/mach7/
# \see https://github.com/solodon4/Mach7
# \see https://github.com/solodon4/SELL
#

GCC_COLORS=always
OS       = $(shell uname -s)
LIBS     = -lstdc++
INCLUDES = -I../../..
ifneq (,$(BOOST_ROOT))
	INCLUDES += -I$(BOOST_ROOT) -DHAS_BOOST
endif
CXXFLAGS = $(INCLUDES) -g -Wall -Wextra -pedantic -v -O2 -DNDEBUG -DXTL_MESSAGE_ENABLED=0 -std=gnu++0x
ifneq (,$(findstring g++,$(CXX)))
	CXXFLAGS += -Wno-vla -fdiagnostics-show-option
# Workaround for MinGW bug: -D__NO_INLINE__
# Not supported by clang on Travis CI machines: -ftrack-macro-expansion=4
endif
ifneq (,$(findstring clang,$(CXX)))
	CXXFLAGS += -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -Wno-vla-extension -Wno-nested-anon-types -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-anonymous-struct
endif

LIBFLAGS =
AR       = ar

.SUFFIXES: .cpp .o

# Create a list of source files.
SOURCES = $(shell ls *.cpp)
# Create a list of object files from the source file lists.
OBJECTS = ${SOURCES:.cpp=.o}
# Create a list of targets.
TARGETS = red-black-tree.exe
# Create a list of dependency files.
DEP_FILES = $(SOURCES:.cpp=.dep)

#include $(DEP_FILES)

# Generic targets

all: $(TARGETS)

# Setting .SECONDARY to blank disables deletion of intermediate files by make
#.SECONDARY:

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
	grep -R --include=*.?pp "#include" * | sed -e "s/^/\"/g" -e "s/:/\"/g" -e "s/\/\/.*//g" -e "s/\"[A-Za-z0-9_\.\/-]*\//\"/g" -e "s/\(<.*>\)/\1\n\1 [color=red,shape=box]/g" -e "s/#include/ -> /g" > includes.dot
# -e "1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }" -e "$$ { s/$$/\n}\n/g }"

# A rule to convert the above GraphViz .dot file into actual picture
includes.png: includes.dot
	dot -Tpng -oincludes.png includes.dot
