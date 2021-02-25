CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=./tp2
DEPENDENCIES=$(shell pwd)/$(shell find . -name '*.c')
ENTRY=main.c

all: $(OUTPUT) mem_test

$(OUTPUT): $(DEPENDENCIES) $(ENTRY)
	gcc $(DEPENDENCIES) $(CFLAGS) -o $(OUTPUT)

mem_test: $(OUTPUT)
	valgrind $(VFLAGS) $(OUTPUT)

debug: $(OUTPUT)
	gdb -tui $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)