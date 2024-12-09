# Compiler and flags

CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iincludes

# Folders
INCLUDES = includes
MODULES = modules
TESTS = tests

# Source files
MODULE_SRCS = $(MODULES)/filteredgreedysearch.cpp $(MODULES)/findmedoid.cpp
MODULE_OBJS = $(MODULE_SRCS:.cpp=.o)

TEST_SRCS = $(TESTS)/filteredgreedysearch_test.cpp $(TESTS)/findmedoid_test.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Executables
TEST_EXECUTABLES = test_filteredgreedysearch test_findmedoid

# Rules
.PHONY: all clean

all: $(TEST_EXECUTABLES)

# Create executables for tests
test_filteredgreedysearch: $(MODULE_OBJS) $(TESTS)/filteredgreedysearch_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_findmedoid: $(MODULE_OBJS) $(TESTS)/findmedoid_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(MODULE_OBJS) $(TEST_OBJS) $(TEST_EXECUTABLES)
