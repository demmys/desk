CC = gcc
CFLAGS = -Wall -O2

TARGET = ../generate.o
OBJS = \
	   constant_pool.o\
	   opcode.o\
	   attributes.o\
	   definition.o\
	   classfile.o\
	   generator.o
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear

all: $(TARGET)
$(TARGET): $(OBJS)
	ld -r -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -I../compile -I../utils -c $<

generator.o: classfile.h
classfile.o: classfile.h constant_pool.h attributes.c base.h

clean:
	rm -f $(OBJS)
clear: clean
	rm -f $(MADE)
