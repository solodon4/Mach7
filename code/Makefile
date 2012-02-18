#
# \file
#
# Makefile for building and testing pattern matching library.
#
# \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
#
# This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
# Copyright (C) 2005-2012 Texas A&M University.
# All rights reserved.
#

#
# Usage:
#     make        - Build all .cpp files in the current folder
#     make syntax - Build all supported library options combination for syntax variations
#     make timing - Build all supported configurations for timing the library
#     make clean  - Clean all targets
#     make test   - Run the test suite
#

OPTFLAGS = $(OPTFLAGS452)
INCLUDES = 
LIBS     = -lstdc++
CXXFLAGS = $(INCLUDES) -Wall -time -O2 -DNDEBUG -std=c++0x \
#-fno-omit-frame-pointer \
#-fno-devirtualize \
#-fno-combine-stack-adjustments \
#-fno-compare-elim \
#-fno-dwarf2-cfi-asm \
#-fno-ipa-profile \
#-fno-keep-inline-dllexport \
#-fno-partial-inlining \
#-fno-prefetch-loop-arrays \
#-fno-strict-volatile-bitfields \
#-fno-tree-bit-ccp \
#-fno-tree-loop-if-convert \
#-fno-asynchronous-unwind-tables \
#-fno-unwind-tables

LIBFLAGS =  
AR       = ar

.SUFFIXES: .cpp .o

# Create a list of source files.
SOURCES  = $(shell ls *.cpp)
# Create a list of object files from the source file lists.
OBJECTS = ${SOURCES:.cpp=.o}     
# Create a list of targets.
TARGETS = ${SOURCES:.cpp=.exe}     

# Build all targets by default
all: 	$(TARGETS)

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
	$(CXX) -o $@ $(LIBFLAGS) $< $(LIBS)
	@echo ================================================================================
	@echo Done building $@
	@echo ================================================================================
	@echo
	@echo

# A rule to build .o file out of a .cpp file
%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# A rule to build all encoding/benchmark/forwarding combinations for timing
#
# Options with performance impact:
# - Default syntax                   \see @XTL_DEFAULT_SYNTAX
# - Use of vtbl frequencies          \see @XTL_USE_VTBL_FREQUENCY
# - Use of memoized_cast             \see @XTL_USE_MEMOIZED_CAST
# - Whether extractors might throw   \see @XTL_EXTRACTORS_MIGHT_THROW
# - Use of static local variables    \see @XTL_PRELOAD_LOCAL_STATIC_VARIABLES
# - Use number of case clauses init  \see @XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
# - Certain under-the-hood types     \see @vtbl_count_t
# - Certain under-the-hood constants \see @XTL_MIN_LOG_SIZE, @XTL_MAX_LOG_SIZE, @XTL_MAX_LOG_INC, @XTL_LOCAL_CACHE_LOG_SIZE, @XTL_IRRELEVANT_VTBL_BITS
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
# - Redundancy checking              \see @XTL_REDUNDANCY_CHECKING
# - Fallthrough behavior             \see @XTL_FALL_THROUGH
# - Use of { & } around case clauses \see @XTL_USE_BRACES
# - Trace of performance             \see @XTL_TRACE_PERFORMANCE
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

# A rule to clean all the intermediates and targets
clean:	
	rm -rf $(TARGETS) $(OBJECTS) *.out *.stackdump *.cmi *.cmx *.obj *.csv time-*.exe syntax-*.exe

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
