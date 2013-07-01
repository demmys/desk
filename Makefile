CC = gcc
CFLAGS = -Wall -O2 -g

TARGET = desk
OBJS = lex.yy.o y.tab.o main.o error.o storage.o compiler.o create.o classfile_constant_pool.o classfile_attributes.o classfile.o generate.o emit.o
MADE = $(TARGET) lex.yy.c y.tab.h y.tab.c
LEX = desk.l
YACC = desk.y

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJS)

clear: clean
	rm -f $(MADE)

.c.o:
	$(CC) $(CFLAGS) -c $<

lex.yy.c: $(LEX) y.tab.h
	lex $<

y.tab.c y.tab.h: $(YACC)
	yacc -d $<

help:
	@echo "USAGE: make [options]"
	@echo "OPTIONS:"
	@echo "  all    compile all files and generate run file."
	@echo "  clean  remove all .o files generated."
	@echo "  clear  remove all generated files."

