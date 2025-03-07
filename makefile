# Makefile for CMOS project

CC = g++
LEX = flex
CFLAGS = -Wall -O2 -std=c++11
TARGET = cmos

all: $(TARGET)

# Generate lex.yy.c from the LEX file.
lex.yy.c: cmos.l
	$(LEX) cmos.l

# Compile the executable from lex.yy.c and cmos.cpp.
$(TARGET): lex.yy.c cmos.cpp
	$(CC) $(CFLAGS) -o $(TARGET) lex.yy.c cmos.cpp

# Clean up generated files.
clean:
	rm -f lex.yy.c $(TARGET)
