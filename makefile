CC = gcc
CFLAGS = -std=c99 -Wall -Wpedantic -Wextra -ggdb3 -Iinclude
VFLAGS = --show-leak-kinds=all --track-origins=yes --leak-check=full

TARGET = acck
OBJS = bin/acck.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -rf acck bin/*.o bin/acck
