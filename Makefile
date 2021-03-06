CC = gcc
CFLAGS = -Wall -O2 -g

TARGET = desk
OBJS = \
	   utils.o\
	   compile.o\
	   generate.o\
	   emit.o\
	   main.o
UTILS = utils/*.c utils/*.h
COMPILE = compile/*.c compile/*.h compile/desk.*
GENERATE = generate/*.c generate/*.h
EMIT = emit/*.c emit/*.h
MADE = $(TARGET)

.SUFFIXES: .c .o
.PHONY: all clean clear help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

utils.o: $(UTILS)
	cd utils; $(MAKE);
compile.o: $(COMPILE)
	cd compile; $(MAKE);
generate.o: $(GENERATE)
	cd generate; $(MAKE);
emit.o: $(EMIT)
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

ls:
	@echo "\n\033[0;31m./\033[0;39m"
	@ls
	@echo "\n\033[0;31mutils\033[0;39m"
	@cd utils; ls;
	@echo "\n\033[0;31mcompile\033[0;39m"
	@cd compile; ls;
	@echo "\n\033[0;31mgenerate\033[0;39m"
	@cd generate; ls;
	@echo "\n\033[0;31memit\033[0;39m"
	@cd emit; ls;
	@echo ""

wc:
	wc main.c compile/*.c compile/*.h compile/desk.* generate/*.c generate/*.h utils/*c utils/*.h emit/*.c emit/*.h

help:
	@echo "USAGE: make [options]"
	@echo "OPTIONS:"
	@echo "  all    compile all files and generate run file."
	@echo "  clean  remove all .o files generated."
	@echo "  clear  remove all generated files."

