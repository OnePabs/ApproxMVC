#Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

#Target executable name
TARGET = approxmvc

# Source and object files
MAXIMAL_MATCHING_SRCS = algorithms/maximal_matching/maximal_matching.c
PITTS_SRCS = algorithms/pitts/pitts.c
SRCS = main.c $(MAXIMAL_MATCHING_SRCS) $(PITTS_SRCS)
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CLFAGS) -c $< -o $@

# Clean rule to remove built files
clean:
	rm -f $(TARGET) $(OBJS)

# Declare non file targets
.PHONY: all clean 