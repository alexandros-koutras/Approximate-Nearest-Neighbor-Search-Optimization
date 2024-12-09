# Compiler and flags

CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iincludes

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
TESTS_OBJ = $(patsubst $(TESTS)/%.cpp,$(TESTS)/%.o,$(TESTS_SRC))

ARGS1 = -i siftsmall/siftsmall_base.fvecs -q siftsmall/siftsmall_query.fvecs \
		-g siftsmall/siftsmall_groundtruth.ivecs -k 100 -l 200 -r 60 -a 1.2

# Executable program
EXEC = project

# Rules
.PHONY: all clean

all: $(TEST_EXECUTABLES)

# Link object files to create the executable
$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $(MODULES_OBJ) $(MAIN_OBJ)

# Compile .cpp files from MODULES into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(MODULES_OBJ)
	$(foreach test_src, $(TESTS_SRC), \
		$(CC) $(CFLAGS) -o $(basename $(test_src)) $(MODULES_OBJ) $(test_src); \
		./$(basename $(test_src)) || exit 1; \
	)

valgrind_tests: $(MODULES_OBJ)
	$(foreach test_src, $(TESTS_SRC), \
		executable=$(basename $(test_src)); \
		$(CC) $(CFLAGS) -o $$executable $(MODULES_OBJ) $(test_src); \
		valgrind --leak-check=full --track-origins=yes ./$$executable || exit 1; \
	)

# Run the executable with additional arguments passed from the command line
run: all
	./$(EXEC) $(ARGS)

run1: all
	./$(EXEC) $(ARGS1)

check: tests

# Run the executable with Valgrind
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(EXEC) $(ARGS)

# Clean the executable files 
clean:
	rm -f $(MODULES_OBJ) $(TESTS_OBJ) $(MAIN_OBJ) $(EXEC) $(basename $(TESTS_SRC))