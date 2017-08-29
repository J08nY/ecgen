####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####


CC ?= gcc
CFLAGS = -Wall --coverage -g -O0
LDFLAGS = -L../../lib/parson -L../../lib/sha1 -L../../lib/pari -L../lib/criterion/build
INCLUDES = -I. -I../../src -I../../lib -I../lib/criterion/include
LIBS = -lrt -lpari -lpthread -lparson -lsha1 -lcriterion

TEST_SRC = $(wildcard *.c) $(wildcard */*.c)
TEST_OBJ = $(patsubst %.c,%.o, $(TEST_SRC))
TESTS = $(patsubst %.c,%, $(TEST_SRC))

ECGEN_SRC = $(wildcard ../../src/*/*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

all: test_ecgen
	LD_LIBRARY_PATH=../lib/criterion/build ./test_ecgen

test_ecgen: $(TEST_OBJ) $(ECGEN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean-all: clean
	rm -f ./test_ecgen

clean:
	find . -type f -name '*.o' -exec rm {} +
	find . -type f -name '*.gcda' -exec rm {} +
	find . -type f -name '*.gcno' -exec rm {} +
	find . -type f -name '*.gcov' -exec rm {} +
