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
# \see https://github.com/solodon4/Mach7
# \see https://github.com/solodon4/SELL
#

#
# Usage:
#     make        - Build all .cpp files in the current folder
#     make all    - Build all the targets and documentation
#     make syntax - Build all supported library options combination for syntax variations
#     make timing - Build all supported configurations for timing the library
#     make cmp    - Build all executables for comparison with other languages
#     make clean  - Clean all targets
#     make doc    - Build Mach7 documentation
#     make includes.png - Build graph representation of header inclusions
#     make test   - Run the test suite
#

GCC_COLORS=always
OS       = $(shell uname -s)
LIBS     = -lstdc++
INCLUDES = -I../..
ifneq (,$(BOOST_ROOT))
	INCLUDES += -I$(BOOST_ROOT) -DHAS_BOOST
endif
# Note: using -std=c++0x instead of -std=gnu++0x fails in standard headers on MinGW
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

.PHONY: all clean cmp default doc syntax tags test timing ver

# Phony targets can have prerequisites. When one phony target is a prerequisite 
# of another, it serves as a subroutine of the other.

# Build all files in current folder as well as all combinations for syntax, timing and comparison
all: default timing cmp

# A phony target should not be a prerequisite of a real target file; if it is, 
# its recipe will be run every time make goes to update that file. 

# Build all targets by default
default: 	$(TARGETS)

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

tags: skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_REP_TEST -o time-fwd-special-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_SEQ_TEST -o time-fwd-special-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_RND_TEST -o time-fwd-special-kind-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_REP_TEST -o time-non-special-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_SEQ_TEST -o time-non-special-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_RND_TEST -o time-non-special-kind-rnd.exe skeleton.cxx

# A rule to build all encoding/benchmark/forwarding combinations for timing
#
# Options with performance impact:
# - Default syntax                   \see #XTL_DEFAULT_SYNTAX
# - Use of vtbl frequencies          \see #XTL_USE_VTBL_FREQUENCY
# - Use of memoized_cast             \see #XTL_USE_MEMOIZED_CAST
# - Whether extractors might throw   \see #XTL_EXTRACTORS_MIGHT_THROW
# - Use of static local variables    \see #XTL_PRELOAD_LOCAL_STATIC_VARIABLES
# - Use number of case clauses init  \see #XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
# - Certain under-the-hood types     \see #vtbl_count_t
# - Certain under-the-hood constants \see #XTL_MIN_LOG_SIZE, #XTL_MAX_LOG_SIZE, #XTL_MAX_LOG_INC, #XTL_LOCAL_CACHE_LOG_SIZE, #XTL_IRRELEVANT_VTBL_BITS
# Most of the combinations of from this set are built with: make timing
timing: skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_REP_TEST -o time-fwd-generic-poly-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_SEQ_TEST -o time-fwd-generic-poly-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_RND_TEST -o time-fwd-generic-poly-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_REP_TEST -o time-fwd-generic-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_SEQ_TEST -o time-fwd-generic-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_RND_TEST -o time-fwd-generic-kind-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_REP_TEST -o time-fwd-special-poly-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_SEQ_TEST -o time-fwd-special-poly-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_RND_TEST -o time-fwd-special-poly-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'F\' -DXTL_REP_TEST -o time-fwd-special-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'F\' -DXTL_SEQ_TEST -o time-fwd-special-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=1 -DXTL_DEFAULT_SYNTAX=\'F\' -DXTL_RND_TEST -o time-fwd-special-kind-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_REP_TEST -o time-non-generic-poly-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_SEQ_TEST -o time-non-generic-poly-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_RND_TEST -o time-non-generic-poly-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_REP_TEST -o time-non-generic-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_SEQ_TEST -o time-non-generic-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_RND_TEST -o time-non-generic-kind-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_REP_TEST -o time-non-special-poly-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_SEQ_TEST -o time-non-special-poly-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_RND_TEST -o time-non-special-poly-rnd.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'K\' -DXTL_REP_TEST -o time-non-special-kind-rep.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'K\' -DXTL_SEQ_TEST -o time-non-special-kind-seq.exe skeleton.cxx
	$(CXX) $(CXXFLAGS) -DXTL_VISITOR_FORWARDING=0 -DXTL_DEFAULT_SYNTAX=\'K\' -DXTL_RND_TEST -o time-non-special-kind-rnd.exe skeleton.cxx

# A rule to build all encoding/syntax combinations to ensure proper semantics
#
# Options with semantic or convenience impact
# - Redundancy checking              \see #XTL_REDUNDANCY_CHECKING
# - Fallthrough behavior             \see #XTL_FALL_THROUGH
# - Use of { & } around case clauses \see #XTL_USE_BRACES
# - Trace of performance             \see #XTL_TRACE_PERFORMANCE
# Most of the combinations of from this set are built with: make syntax
syntax: syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft0-br1-S-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft0-br1-S-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft0-br1-P-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft0-br1-P-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft0-br1-E-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft0-br1-E-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft0-br1-F-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft0-br1-F-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft0-br1-U-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft0-br1-U-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft0-br1-K-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft0-br1-K-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft1-br1-S-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft1-br1-S-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft1-br1-P-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft1-br1-P-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft1-br1-E-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft1-br1-E-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft1-br1-F-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft1-br1-F-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft1-br1-U-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft1-br1-U-gen.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft1-br1-K-spe.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft1-br1-K-gen.exe syntax.cxx

# An older rule to build all encoding/syntax combinations that had different ordering
oldsyntax: syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft0-br0-spe-S.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft0-br0-spe-P.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft0-br0-spe-E.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft0-br0-spe-F.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft0-br0-spe-U.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft0-br0-spe-K.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft0-br0-gen-s.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft0-br0-gen-p.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft0-br0-gen-e.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft0-br0-gen-f.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft0-br0-gen-u.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft0-br0-gen-k.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft0-br1-spe-S.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft0-br1-spe-P.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft0-br1-spe-E.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft0-br1-spe-F.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft0-br1-spe-U.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft0-br1-spe-K.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft0-br1-gen-s.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft0-br1-gen-p.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft0-br1-gen-e.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft0-br1-gen-f.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft0-br1-gen-u.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=0 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft0-br1-gen-k.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft1-br0-spe-S.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft1-br0-spe-P.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft1-br0-spe-E.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft1-br0-spe-F.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft1-br0-spe-U.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft1-br0-spe-K.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft1-br0-gen-s.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft1-br0-gen-p.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft1-br0-gen-e.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft1-br0-gen-f.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft1-br0-gen-u.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=0 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft1-br0-gen-k.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'S\' -o syntax-rc0-ft1-br1-spe-S.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'P\' -o syntax-rc0-ft1-br1-spe-P.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'E\' -o syntax-rc0-ft1-br1-spe-E.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'F\' -o syntax-rc0-ft1-br1-spe-F.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'U\' -o syntax-rc0-ft1-br1-spe-U.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'K\' -o syntax-rc0-ft1-br1-spe-K.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'s\' -o syntax-rc0-ft1-br1-gen-s.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'p\' -o syntax-rc0-ft1-br1-gen-p.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'e\' -o syntax-rc0-ft1-br1-gen-e.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'f\' -o syntax-rc0-ft1-br1-gen-f.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'u\' -o syntax-rc0-ft1-br1-gen-u.exe syntax.cxx
	$(CXX) $(CXXFLAGS) -DXTL_REDUNDANCY_CHECKING_XXX -DXTL_FALL_THROUGH=1 -DXTL_USE_BRACES=1 -DXTL_DEFAULT_SYNTAX=\'k\' -o syntax-rc0-ft1-br1-gen-k.exe syntax.cxx

# A rule to build all executables for comparison with other languages
cmp: cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_SEQ_TEST -o cmp-non-generic-poly-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_SEQ_TEST -o cmp-non-generic-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_SEQ_TEST -o cmp-fwd-generic-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_SEQ_TEST -o cmp-non-special-poly-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'K\' -DXTL_SEQ_TEST -o cmp-non-special-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'F\' -DXTL_SEQ_TEST -o cmp-fwd-special-kind-seq.exe cmp_cpp.cxx

# A rule to build Haskell executables for comparison with other languages
# TODO: Make a generic .hs rule
hs: cmp_haskell.hs
	ghc -O --make cmp_haskell

# A rule to build OCaml executables for comparison with other languages
# TODO: Make a generic .ml rule
caml: cmp_ocaml.ml 
	ocamlopt.opt unix.cmxa -o cmp_ocaml.exe cmp_ocaml.ml

# A rule to clean all the intermediates and targets
clean:
	rm -rf $(TARGETS) $(OBJECTS) *.out *.stackdump *.cmi *.cmx *.obj *.csv *.exe.dSYM time-*.exe syntax-*.exe cmp-*.exe cmp_*.exe cmp_*.hi cmp_*.o

# A rule to create GraphViz .dot file with graph representation of header inclusions
includes.dot:
	grep -R --include=*.hpp "#include" * | sed -e "s/^/\"/g" -e "s/:/\"/g" -e "s/\/\/.*//g" -e "s/\"[A-Za-z0-9_\.\/-]*\//\"/g" -e "s/\(<.*>\)/\1\n\1 [color=red,shape=box]/g" -e "s/#include/ -> /g" > includes.dot

# -e "1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }" -e "$$ { s/$$/\n}\n/g }"

# A rule to convert the above GraphViz .dot file into actual picture
includes.png: includes.dot
	dot -Tpng -oincludes.png includes.dot

# Run those examples for which there are correct_output/*.out files and check that output is the same
check:
	@for file in *.exe; do \
		name=`basename $$file .exe` ; \
		name=`basename $$name .crash` ; \
		if (ls correct_output/$$name.out >/dev/null 2>&1) ; then \
			echo dummy >/dev/null 2>&1; \
		else \
			continue ; \
		fi ; \
		echo ======================================== [ $$file ] ; \
		./$$file > $$name.out ; \
		if (diff -q --strip-trailing-cr $$name.out correct_output/$$name.out) ; then \
			echo Match >/dev/null 2>&1 ; \
		else \
			echo Dont match >/dev/null 2>&1 ; \
		fi ; \
	done ;

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
