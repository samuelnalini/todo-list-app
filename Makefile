TARGET = main
CC = g++
CFLAGS = -std=c++23 -g -Wall
SRC_DIR = src

.PHONY: default all clean

default: $(TARGET)
all: default

main: $(SRC_DIR)/*.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/*.cpp -o $(TARGET)

clean:
	rm -rf $(TARGET)
