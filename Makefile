default: main

CC=clang
CFLAGS= -Wall -Wextra -ggdb

SOURCE_DIR=./src
CARS_DIR=$(SOURCE_DIR)/cars
BUILD_PATH=./build
RAYLIB_DIR=./raylib-5.0_linux_amd64

$(shell test -d $(BUILD_PATH) || mkdir -p $(BUILD_PATH))

SOURCES=$(wildcard $(SOURCE_DIR)/*.c $(CARS_DIR)/*.c)

main: $(SOURCES)
	$(info SOURCES => $(SOURCES))
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$@ -xc $^ -I./include -L$(RAYLIB_DIR)/lib/ -I$(RAYLIB_DIR)/include -l:libraylib.a -lm

clean: 
	rm -rf $(BUILD_PATH)
