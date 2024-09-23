# Compiler and Linking Variables
CC = gcc
CFLAGS = -Wall -fPIC -I./include  # Include directory where memory_manager.h is located
LIB_NAME = libmemory_manager.so

# Source and Object Files
SRC = memory_manager.c
OBJ = $(SRC:.c=.o)

# Default target: Build the memory manager library and test program
all: mmanager test_mmanager

# Rule to create the dynamic library
$(LIB_NAME): $(OBJ)
	$(CC) -shared -o $@ $(OBJ)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the memory manager
mmanager: $(LIB_NAME)

# Test target to run the memory manager test program
test_mmanager: $(LIB_NAME)
	$(CC) -o test_memory_manager test_memory_manager.c -L. -lmemory_manager

# Run test cases for the memory manager
run_test_mmanager:
	./test_memory_manager

# Clean target to clean up build files
clean:
	rm -f $(OBJ) $(LIB_NAME) test_memory_manager
