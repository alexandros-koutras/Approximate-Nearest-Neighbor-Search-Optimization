# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iincludes

# Folders
INCLUDES = includes
MODULES = modules
TESTS = tests

# Source files
MODULE_SRCS = $(MODULES)/greedysearch.cpp $(MODULES)/filteredgreedysearch.cpp
MODULE_OBJS = $(MODULE_SRCS:.cpp=.o)

TEST_SRCS = $(TESTS)/filteredgreedysearch_test.cpp $(TESTS)/greedysearch_test.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Executables
TEST_EXECUTABLES = test_greedysearch test_filteredgreedysearch

# Rules
.PHONY: all clean

all: $(TEST_EXECUTABLES)

# Create executables for tests
test_greedysearch: $(MODULE_OBJS) $(TESTS)/greedysearch_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_filteredgreedysearch: $(MODULE_OBJS) $(TESTS)/filteredgreedysearch_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(MODULE_OBJS) $(TEST_OBJS) $(TEST_EXECUTABLES)
