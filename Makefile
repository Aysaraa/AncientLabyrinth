# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Object files
OBJS = main.o game.o

# Target executable
TARGET = game

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.c into main.o
main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

# Rule to compile game.c into game.o
game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

# Clean up object and executable files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the game
run: $(TARGET)
	./$(TARGET)