.PHONY: default all clean pch single-header single-header/ctc.hpp

default: all

CXX_STANDARD := 20

PYTHON := python3

CXX_IS_CLANG := $(shell $(CXX) --version 2>/dev/null | grep -qi clang && echo yes)

override CXXFLAGS := $(CXXFLAGS) -std=c++$(CXX_STANDARD) -Iinclude -O2 -pedantic -Wall -Wextra -Werror -Wconversion

# precompiled header: the library templates are parsed once here
# instead of once per translation unit
ifeq ($(CXX_IS_CLANG),yes)
PCH := ctc.pch
PCH_USE = -include-pch $(PCH)
else
PCH := include/ctc.hpp.gch
PCH_USE =
endif

TESTS := $(wildcard tests/*.cpp)
OBJECTS := $(TESTS:%.cpp=%.o)
DEPENDENCY_FILES := $(OBJECTS:%.o=%.d)

all: $(OBJECTS)

$(OBJECTS): %.o: %.cpp $(PCH)
	$(CXX) $(CXXFLAGS) $(PCH_USE) -MMD -c $< -o $@

pch: $(PCH)

$(PCH): include/ctc.hpp
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@

-include $(DEPENDENCY_FILES)

clean:
	rm -f $(OBJECTS) $(DEPENDENCY_FILES) ctc.pch include/ctc.hpp.gch

# needs python3 with the quom package
single-header: single-header/ctc.hpp

single-header/ctc.hpp:
	$(PYTHON) -m quom include/ctc.hpp ctc.hpp.tmp
	echo "/*" > single-header/ctc.hpp
	cat LICENSE >> single-header/ctc.hpp
	echo "*/" >> single-header/ctc.hpp
	cat ctc.hpp.tmp >> single-header/ctc.hpp
	rm ctc.hpp.tmp
