CC = gcc
CFLAGS = -Wall -O2

TARGET = desk
OBJS = lex.yy.o
MADE = $(TARGET) lex.yy.c
LEX = desk_.l

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJS)

clear: clean
	rm -f $(MADE)

.c.o:
	$(CC) $(CFLAGS) -c $<

lex.yy.c: $(LEX)
	lex $<
