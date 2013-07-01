CC = gcc
CFLAGS = -Wall -O2

TARGET = desk
OBJS = \
	   main.o\
	   utils.o\
	   compile.o\
	   generate.o\
	   emit.o
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear help

all: $(TARGET)

$(TARGET): $(OBJS)
	cd utils; $(MAKE);
	cd compile; $(MAKE);
	cd generate; $(MAKE);
	cd emit; $(MAKE);
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -Iutils -Icompile -Igenerate -Iemit $<

clean:
	cd utils; $(MAKE) clean;
	cd compile; $(MAKE) clean;
	cd generate; $(MAKE) clean;
	cd emit; $(MAKE) clean;
	rm -f $(OBJS)

clear: clean
	cd utils; $(MAKE) clear;
	cd compile; $(MAKE) clear;
	cd generate; $(MAKE) clear;
	cd emit; $(MAKE) clear;
	rm -f $(MADE)

help:
	@echo "USAGE: make [options]"
	@echo "OPTIONS:"
	@echo "  all    compile all files and generate run file."
	@echo "  clean  remove all .o files generated."
	@echo "  clear  remove all generated files."

