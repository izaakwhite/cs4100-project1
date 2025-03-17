# Makefile for CMOS project

CC = g++
LEX = flex
CFLAGS = -std=c++17
LFLAGS = -lfl
TARGET = cmos
SCANNER = scanner

all: $(SCANNER) $(TARGET)

# Generate lex.yy.c from the LEX file.
lex.yy.c: cmos.l
	$(LEX) cmos.l

# Compile the scanner executable from lex.yy.c.
$(SCANNER): lex.yy.c
	$(CC) $(CFLAGS) -o $(SCANNER) lex.yy.c $(LFLAGS)

# Compile the cmos executable from main.cpp and cmos.cpp.
$(TARGET): cmos.cpp
	$(CC) $(CFLAGS) -o $(TARGET) cmos.cpp

# Clean up generated files.
clean:
	rm -f lex.yy.c $(SCANNER) $(TARGET)