#
# \file
#
# Makefile for building and testing pattern matching library.
#
# \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
# Copyright (C) 2011-2013 Texas A&M University.
# All rights reserved.
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

OS       = $(shell uname -s)
LIBS     = -lstdc++
INCLUDES = 
CXXFLAGS = $(INCLUDES) -Wall -Wextra -pedantic -time -s -O2 -DNDEBUG -DXTL_MESSAGE_ENABLED=0 -std=c++0x \
#-fno-omit-frame-pointer \
#-fno-rtti
LIBFLAGS = $(LIBS)
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
all: default syntax timing cmp

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
cmp: cmp_cpp.cxx cmp_ocaml.ml cmp_haskell.hs
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'p\' -DXTL_SEQ_TEST -o cmp-non-generic-poly-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'k\' -DXTL_SEQ_TEST -o cmp-non-generic-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'f\' -DXTL_SEQ_TEST -o cmp-fwd-generic-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'P\' -DXTL_SEQ_TEST -o cmp-non-special-poly-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'K\' -DXTL_SEQ_TEST -o cmp-non-special-kind-seq.exe cmp_cpp.cxx
	$(CXX) $(CXXFLAGS) -DXTL_DEFAULT_SYNTAX=\'F\' -DXTL_SEQ_TEST -o cmp-fwd-special-kind-seq.exe cmp_cpp.cxx
	ghc -O --make cmp_haskell

caml: cmp_ocaml.ml 
	ocamlopt.opt unix.cmxa -o cmp_ocaml.exe cmp_ocaml.ml

# A rule to clean all the intermediates and targets
clean:
	rm -rf $(TARGETS) $(OBJECTS) *.out *.stackdump *.cmi *.cmx *.obj *.csv time-*.exe syntax-*.exe cmp-*.exe cmp_*.exe cmp_*.hi cmp_*.o

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
