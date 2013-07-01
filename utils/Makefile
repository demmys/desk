CC = gcc
CFLAGS = -Wall -O2

TARGET = ../utils.o
OBJS = error.o storage.o
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear

all: $(TARGET)
$(TARGET): $(OBJS)
	ld -r -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
clear: clean
	rm -f $(MADE)
