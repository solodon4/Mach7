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
# This file is a part of Mach7 library test suite.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# \see https://parasol.tamu.edu/mach7/
# \see https://github.com/solodon4/Mach7
# \see https://github.com/solodon4/SELL
#


GCC_COLORS=always
OS      = $(shell uname -s)
TARGETS = couparser.exe
SOURCES = $(shell ls *.cpp) cou.tab.cpp cou.yy.cpp
#CXX     = g++
LIBS     = -lstdc++
INCLUDES = -I../..
ifneq (,$(BOOST_ROOT))
	INCLUDES += -I$(BOOST_ROOT) -DHAS_BOOST
endif
CXXFLAGS = $(INCLUDES) -g -Wall -Wextra -pedantic -v -O2 -DNDEBUG -DXTL_MESSAGE_ENABLED=0 -std=gnu++0x
ifneq (,$(findstring g++,$(CXX)))
	CXXFLAGS += -Wno-vla -ftrack-macro-expansion=4 -fdiagnostics-show-option -D__NO_INLINE__ 
endif
ifneq (,$(findstring clang,$(CXX)))
	CXXFLAGS += -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -Wno-vla-extension -Wno-nested-anon-types -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-anonymous-struct
endif

LIBFLAGS = $(LIBS)
AR       = ar

.SUFFIXES: .cpp .o

# Create lists of object files from the source file lists.

OBJECTS = ${SOURCES:.cpp=.o}

DEP_FILES = $(SOURCES:.cpp=.dep)

# Setting .SECONDARY to blank disables deletion of intermediate files by make
#.SECONDARY:

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
%.o: %.cpp *.hpp
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
