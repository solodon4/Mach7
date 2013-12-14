TARGETS = couparser.exe
SOURCES = $(shell ls *.cpp) cou.tab.cpp cou.yy.cpp
OS      = $(shell uname -s)
CXX     = g++
EXTRAFLAGS = -Wall -Wno-long-long -ftemplate-depth-250

ifeq ($(OS),Darwin)
EXTRAFLAGS = $(EXTRAFLAGS) -Wno-long-double
endif

INCLUDES = 
LIBS     = -lstdc++
CXXFLAGS = $(INCLUDES) -Wall -time -O2 -DNDEBUG -std=c++0x $(EXTRAFLAGS)
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

%.exe: $(OBJECTS)
	@echo --------------------------------------------------------------------------------
	@echo Linking $@ ...
	$(CXX) -o $@ *.o $(LIBFLAGS)
	@echo ================================================================================
	@echo Done building $@
	@echo ================================================================================
	@echo
	@echo

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
