CFLAGS= -Wall -Wextra -ggdb -I./include

# linux
CC=clang
RAYLIB_DIR=./raylib-5.5_linux_amd64
RAYLIB_CFLAGS= -l:libraylib.a -lm -I$(RAYLIB_DIR)/include/ -L$(RAYLIB_DIR)/lib/ 

# windows
CC_W=x86_64-w64-mingw32-gcc
RAYLIB_DIR_W=./raylib-5.5_win64_mingw-w64
RAYLIB_CFLAGS_W= -l:libraylib.a -lm -I$(RAYLIB_DIR_W)/include/ -L$(RAYLIB_DIR_W)/lib/ 

$(info CC => $(CC))
$(info RAYLIB_CFLAGS => $(RAYLIB_CFLAGS))
$(info ---------------------------------)
$(info CC_W => $(CC_W))
$(info RAYLIB_CFLAGS_W => $(RAYLIB_CFLAGS_W))
$(info ---------------------------------)

SOURCE_DIR=./src
CARS_DIR=$(SOURCE_DIR)/cars
BUILD_PATH=./build

$(shell test -d $(BUILD_PATH) || mkdir -p $(BUILD_PATH))

SOURCES=$(wildcard $(SOURCE_DIR)/*.c $(CARS_DIR)/*.c)

main: $(SOURCES)
	$(info SOURCES => $(SOURCES))
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$@ -xc $^ $(RAYLIB_CFLAGS)

win: $(SOURCES)
	$(CC_W) $(CFLAGS) -o $(BUILD_PATH)/$@.exe $^ $(RAYLIB_CFLAGS_W) -static -lwinmm -lgdi32 

clean: 
	rm -rf $(BUILD_PATH)

