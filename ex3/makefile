# Name of the executable
TARGET = bash_mini

# Source file
SRC = bash_mini.c

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Default target: compile the program
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Run the program (build first if needed)
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET)
