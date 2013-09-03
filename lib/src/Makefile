JAVAC = javac
MAIN = Sample.java
TARGET = ../classes
CFLAGS = -J-Dfile.encoding=UTF8 -d $(TARGET)

all:
	@if [ ! -d ../classes ]; then \
	mkdir ../classes ; \
	fi
	$(JAVAC) $(MAIN) $(CFLAGS)

clean:
	rm -rf ../classes
