CC = gcc
CFLAGS = -Wall -O2

TARGET = dpfc
OBJS = lex.yy.o
MADE = $(TARGET) lex.yy.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJS)

clear: clean
	rm -f $(MADE)

.c.o:
	$(CC) $(CFLAGS) -c $<

lex.yy.c: dpfc_.l
	lex dpfc_.l
