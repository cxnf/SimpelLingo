SOURCE=$(wildcard src/*.c)
OBJECT=$(patsubst src/%.c,obj/%.o,$(SOURCE))
EXEC="bin/exec"

all: $(OBJECT)
	gcc -Wall -g -o $(EXEC) $^

obj/%.o: src/%.c
	gcc -Wall -g -c -o $@ $<
