# Compiler and flags
CC = g++
CFLAGS = -Wall -Werror -g -I$(INCLUDE)

# Paths
MODULES = ./modules
TESTS = ./tests
INCLUDE = ./include

# Automatically find all .cpp files and convert them to .o
MAIN_SRC = main.cpp
MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))
MODULES_SRC = $(wildcard $(MODULES)/*.cpp)
MODULES_OBJ = $(patsubst $(MODULES)/%.cpp,$(MODULES)/%.o,$(MODULES_SRC))
TESTS_SRC = $(wildcard $(TESTS)/*.cpp)
TESTS_OBJ = $(patsubst $(TESTS)/%.cpp,$(TESTS)/%.o,$(TESTS_SRC))

# Executable program
TEST_EXEC = run_tests
EXEC = project

# Default rule
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $(MODULES_OBJ) $(MAIN_OBJ)

# Compile .cpp files from MODULES into .o files
$(MODULES)/%.o: $(MODULES)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(MODULES_OBJ)
	$(foreach test_src, $(TESTS_SRC), \
		$(CC) $(CFLAGS) -o $(basename $(test_src)) $(MODULES_OBJ) $(test_src); \
		./$(basename $(test_src)) || echo "$(test_src) failed."; \
	)

# Run the executable with additional arguments passed from the command line
run: all
	@./$(EXEC) $(ARGS)

# Clean the executable files 
clean:
	rm -f $(MODULES_OBJ) $(TESTS_OBJ) $(MAIN_OBJ) $(EXEC) $(basename $(TESTS_SRC))

