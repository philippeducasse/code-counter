CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I include

SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/config.c $(SRC_DIR)/counter.c $(SRC_DIR)/filter.c $(SRC_DIR)/walker.c
HEADERS = $(INCLUDE_DIR)/config.h $(INCLUDE_DIR)/counter.h $(INCLUDE_DIR)/filter.h $(INCLUDE_DIR)/walker.h

.PHONY: build test clean

build: loc

loc: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o loc $(SOURCES)

test: $(TEST_DIR)/test.c $(SRC_DIR)/config.c $(SRC_DIR)/counter.c $(SRC_DIR)/filter.c $(SRC_DIR)/walker.c
	$(CC) $(CFLAGS) -o test_runner $(TEST_DIR)/test.c $(SRC_DIR)/config.c $(SRC_DIR)/counter.c $(SRC_DIR)/filter.c $(SRC_DIR)/walker.c
	./test_runner

clean:
	rm -f loc test_runner /tmp/test_*.txt