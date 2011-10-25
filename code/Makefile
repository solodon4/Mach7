#
# This is a common Makefile for code examples from the book
# "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
#

#
# Usage:
#     make        - Build all examples
#     make clean  - Clean all examples
#     make test   - Run the test suite
#

INCLUDES = -I"$(BOOST)"
LIBS     = -lstdc++
CXXFLAGS = $(INCLUDES) -Wall -time -O2 -DNDEBUG -std=gnu++0x
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

# A rule to clean all the intermediates and targets
clean:	
	rm -rf $(TARGETS) $(OBJECTS) *.out *.stackdump *.cmi *.cmx *.obj *.csv time-*.exe

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
	      ./$$file ; \
	    fi \
	done ; \
	echo -n 
