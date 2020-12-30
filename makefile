CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=tp2
DEPENDENCIES=/Users/lucianotrujillo/Development/C/tp_2/algo2tp2/*.c /Users/lucianotrujillo/Development/C/tp_2/algo2tp2/**/*.c /Users/lucianotrujillo/Development/C/tp_2/algo2tp2/**/**/*.c
ENTRY=main.c

all: $(OUTPUT)

$(OUTPUT): $(DEPENDENCIES) $(ENTRY)
	gcc $(DEPENDENCIES) $(CFLAGS) -o $(OUTPUT)

mem_test: $(OUTPUT)
	valgrind $(VFLAGS) ./$(OUTPUT)

debug: $(OUTPUT)
	gdb -tui $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)