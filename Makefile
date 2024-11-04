# Compiler and flags
CC = g++
CFLAGS = -Wall -Werror -g -I$(INCLUDE)

# Paths
MODULES = ./modules
TESTS = ./tests
INCLUDE = ./include

# Automatically find all .cpp files and convert them to .o
MODULES_SRC = $(wildcard $(MODULES)/*.cpp)
TESTS_SRC = $(wildcard $(TESTS)/*.cpp)
MODULES_OBJ = $(patsubst $(MODULES)/%.cpp,$(MODULES)/%.o,$(MODULES_SRC))
TESTS_OBJ = $(patsubst $(TESTS)/%.cpp,$(TESTS)/%.o,$(TESTS_SRC))

# Executable program
EXEC = out
TEST_EXEC = run_tests  # Name for the test executable

# Default rule
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(MODULES_OBJ)
	$(CC) -o $@ $(MODULES_OBJ)

# Compile .cpp files from MODULES into .o files
$(MODULES)/%.o: $(MODULES)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile and run tests
tests: $(MODULES_OBJ) $(TESTS_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(MODULES_OBJ) $(TESTS_OBJ)
	./$(TEST_EXEC)  # Run the compiled test executable

# Compile .cpp files from TESTS into .o files
$(TESTS)/%.o: $(TESTS)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Run the executable with additional arguments passed from the command line
run: all
	./$(EXEC) $(ARGS)

# Clean the build directory
clean:
	rm -f $(MODULES_OBJ) $(TESTS_OBJ) $(EXEC) $(TEST_EXEC)
	clear
