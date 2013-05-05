CC = gcc
CFLAGS = -Wall -O2

TARGET = desk
OBJS = y.tab.o lex.yy.o main.o compiler.o create.o storage.o
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

y.tab.h: $(YACC)
	yacc -d $<
