CC = gcc
CFLAGS = -Wall -O2

TARGET = ../generate.o
OBJS = \
	   classfile_constant_pool.o\
	   classfile_opcode.o\
	   classfile_attributes.o\
	   classfile_definition.o\
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
classfile.o: classfile.h classfile_constant_pool.h classfile_attributes.c classfile_base.h

clean:
	rm -f $(OBJS)
clear: clean
	rm -f $(MADE)
