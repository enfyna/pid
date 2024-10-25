default: main

CC=clang
CFLAGS= -Wall -Wextra

SHELL := /bin/bash
$(shell mkdir -p build)

main: ./src/main.c graph
	$(CC) $(CFLAGS) -o ./build/$@ $< ./build/graph.o -I./include -L./raylib-5.0_linux_amd64/lib/ -I./raylib-5.0_linux_amd64/include -l:libraylib.a -lm

graph: ./src/graph.c
	$(CC) $(CFLAGS) -xc $^ -c -o ./build/graph.o -I./include -I./raylib-5.0_linux_amd64/include/

clean: 
	rm -rf build
