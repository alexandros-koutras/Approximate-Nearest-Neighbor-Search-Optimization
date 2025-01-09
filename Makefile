# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I$(INCLUDES)

# Folders
INCLUDES = includes
MODULES = modules
TESTS = tests

# Automatically find all .cpp files and convert them to .o
MAIN_SRC = main.cpp
MODULES_SRC = $(wildcard $(MODULES)/*.cpp)
TESTS_SRC = $(wildcard $(TESTS)/*.cpp)

MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))
MODULES_OBJ = $(patsubst $(MODULES)/%.cpp,$(MODULES)/%.o,$(MODULES_SRC))
TESTS_EXECUTABLES = $(patsubst %.cpp,%,$(TESTS_SRC))

ARGS1 = -i datasets/dummy-data.bin -q datasets/dummy-queries.bin \
        -g datasets/dummy-groundtruth.bin -k 100 -l 120 -r 60 -a 1.2 \
		-s no -f stitched -t 50

ARGS2 = -i datasets/dummy-data.bin -q datasets/dummy-queries.bin \
        -g datasets/dummy-groundtruth.bin -k 100 -l 120 -r 60 -a 1.2 \
		-s graph.bin -f stitched -t 50

ARGS3 = -i datasets/dummy-data.bin -q datasets/dummy-queries.bin \
        -g datasets/dummy-groundtruth.bin -k 100 -l 120 -r 60 -a 1.2 \
		-z aa -w false -t 50

ARGS4 = -i datasets/dummy-data.bin -q datasets/dummy-queries.bin \
        -g datasets/dummy-groundtruth.bin -k 100 -l 120 -r 60 -a 1.2 \
		-z graph.bin -w false -t 50

# Executable program
EXEC = project

# Rules
.PHONY: all clean tests valgrind_tests check run run1

all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp files to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run tests
tests: $(TESTS_EXECUTABLES)
	@$(foreach test,$(TESTS_EXECUTABLES), ./$(test) || exit 1;)

valgrind_tests: $(TESTS_EXECUTABLES)
	@$(foreach test,$(TESTS_EXECUTABLES), \
		valgrind --leak-check=full --track-origins=yes ./$(test) || exit 1;)

# Build test executables
$(TESTS_EXECUTABLES): % : %.cpp $(MODULES_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(MODULES_OBJ) $<

# Run the executable with arguments
run: $(EXEC)
	./$(EXEC) $(ARGS)

run1: $(EXEC)
	./$(EXEC) $(ARGS1)

run2: $(EXEC)
	./$(EXEC) $(ARGS2)

run3: $(EXEC)
	./$(EXEC) $(ARGS3)

run4: $(EXEC)
	./$(EXEC) $(ARGS4)

check: tests

# Run the executable with Valgrind
valgrind: $(EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(EXEC) $(ARGS)

# Clean the build
clean:
	rm -f $(MODULES_OBJ) $(MAIN_OBJ) $(EXEC) $(TESTS_EXECUTABLES)