####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####


CC ?= gcc
CFLAGS = -Wall
TEST ?= 0
ifeq ($(TEST), 1)
	CFLAGS += --coverage -g -O0
endif
LDFLAGS = -L../../lib/parson -L../../lib/sha1 -L../../lib/pari -L../lib/criterion/build
INCLUDES = -I. -I../../src -I../../lib -I../lib/criterion/include
LIBS = -lrt -lpari -lpthread -lparson -lsha1 -lcriterion

TEST_SRC = $(wildcard *.c) $(wildcard */*.c)
TEST_HDR = $(wildcard */*.h)
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

clean-all: clean clean-cov
	rm -f ./test_ecgen

clean:
	find . -type f -name '*.o' -exec rm {} +

clean-cov:
	find . -type f -name '*.gcda' -exec rm {} +
	find . -type f -name '*.gcno' -exec rm {} +
	find . -type f -name '*.gcov' -exec rm {} +

format:
	clang-format -i $(TEST_SRC)
	clang-format -i $(TEST_HDR)