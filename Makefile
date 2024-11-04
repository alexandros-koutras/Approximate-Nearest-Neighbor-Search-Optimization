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

# Main program source file with main function
MAIN_SRC = main.cpp  # Make sure this file exists and includes the main function

# Executable program
EXEC = out

# Default rule
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(MODULES_OBJ) $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $@ $(MODULES_OBJ) $(MAIN_SRC)

# Compile .cpp files from MODULES into .o files
$(MODULES)/%.o: $(MODULES)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Run the executable with additional arguments passed from the command line
run: all
	@./$(EXEC) $(ARGS)

