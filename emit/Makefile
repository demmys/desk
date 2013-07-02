CC = gcc
CFLAGS = -Wall -O2

TARGET = ../emit.o
OBJS = emitter.o
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear

all: $(TARGET)
$(TARGET): $(OBJS)
	ld -r -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -I../generate -I../compile -I../utils -c $<

emitter.o: emitter.h ../generate/generator.h

clean:
	rm -f $(OBJS)
clear: clean
	rm -f $(MADE)
