EXECUTABLE := main

CC := gcc
CFLAGS := -Wall -g

SRC_DIR := src
BUILD_DIR := build
LIBS_DIR := lib
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.c")

all: clean $(BUILD_DIR)/${EXECUTABLE}

$(BUILD_DIR)/${EXECUTABLE}:
	$(CC) $(CFLAGS) $(SOURCES) -L$(LIBS_DIR) -lutils -o $(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -rf build/*
