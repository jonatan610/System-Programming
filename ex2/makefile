# Makefile
CC = gcc
CFLAGS = -Wall -g
TARGET = my_copy

all: $(TARGET)

$(TARGET): my_copy.c
	$(CC) $(CFLAGS) my_copy.c -o $(TARGET)

clean:
	rm -f $(TARGET)