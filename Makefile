CC = gcc
CFLAGS = -Wall -O2

TARGET = desk
OBJS = \
	   utils.o\
	   compile.o\
	   generate.o\
	   emit.o\
	   main.o
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

utils.o:
	cd utils; $(MAKE);
compile.o:
	cd compile; $(MAKE);
generate.o:
	cd generate; $(MAKE);
emit.o:
	cd emit; $(MAKE);

main.o: main.c emit/emitter.h compile/y.tab.c
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

