# CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
# VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
# OUTPUT=tp2
# DEPENDENCIES=$(shell find . -name '*.c')
# ENTRY=main.c

# THIS_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
# BIN = $(SRC)/bin

# all: $(OUTPUT) mem_test

# $(OUTPUT): $(DEPENDENCIES) $(ENTRY)
# 	docker run --workdir $(HOME) --entrypoint $(CC) -v $(THIS_DIR):$(HOME) mooreryan/valgrind $(CFLAGS) -o $(OUTPUT) $(DEPENDENCIES)
# 	#gcc $(DEPENDENCIES) $(CFLAGS) -o $(OUTPUT)

# mem_test: $(OUTPUT)
# 	docker run --workdir $(HOME) -v $(THIS_DIR):$(HOME) mooreryan/valgrind ./$(OUTPUT)

# debug: $(OUTPUT)
# 	gdb -tui $(OUTPUT)

# clean:
# 	rm -f *.o $(OUTPUT)
CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=$(shell pwd)/tp2
DEPENDENCIES=$(shell pwd)/$(shell find . -name '*.c')
ENTRY=main.c

all: $(OUTPUT)

$(OUTPUT): $(DEPENDENCIES) $(ENTRY)
	gcc $(DEPENDENCIES) $(CFLAGS) -o $(OUTPUT)

mem_test: $(OUTPUT)
	valgrind $(VFLAGS) $(OUTPUT)

debug: $(OUTPUT)
	gdb -tui $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)