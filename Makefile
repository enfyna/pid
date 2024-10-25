default: main

CC=clang
CFLAGS= -Wall -Wextra

BUILD_PATH=./build
RAYLIB_DIR=./raylib-5.0_linux_amd64

$(shell test -d $(BUILD_PATH) || mkdir -p $(BUILD_PATH))

main: ./src/main.c $(BUILD_PATH)/graph.o
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$@ $^ -I./include -L$(RAYLIB_DIR)/lib/ -I$(RAYLIB_DIR)/include -l:libraylib.a -lm

$(BUILD_PATH)/graph.o: ./src/graph.c
	$(CC) $(CFLAGS) -xc $^ -c -o $@ -I./include -I$(RAYLIB_DIR)/include/

clean: 
	rm -rf build
