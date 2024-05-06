# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = wordle.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = wordle

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $@

# Rule to compile source files
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
    $(RM) $(OBJS) $(TARGET)
