CC = gcc
CFLAGS = -Wall -O2

TARGET = ../compile.o
OBJS = \
	   compiler.o\
	   create.o\
	   lex.yy.o\
	   y.tab.o
MADE = \
	   $(TARGET)\
	   lex.yy.c\
	   y.tab.h\
	   y.tab.c
LEX = desk.l
YACC = desk.y

.SUFFIXES: .c .o
.PHONY: all clean clear

all: $(TARGET)
$(TARGET): $(OBJS)
	ld -r -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -I../utils -c $<

create.o: compiler.h
compiler.o: compiler.h ../utils/storage.h ../utils/error.h

lex.yy.c: $(LEX) y.tab.h create.h
	lex $<
y.tab.c y.tab.h: $(YACC) create.h
	yacc -d $<

clean:
	rm -f $(OBJS)
clear: clean
	rm -f $(MADE)
