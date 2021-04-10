####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017-2018,2021 J08nY
#
####


CC ?= gcc
TEST ?= 0
ifeq ($(TEST), 1)
    ECGEN_CFLAGS = --coverage -g -O0
endif
ECGEN_LDFLAGS = -L../../lib/parson -L../../lib/sha1 -L../../lib/pari -L../lib/criterion/build
ECGEN_INCLUDES = -I. -I../../src -I../../lib -I../lib/criterion/include
ECGEN_LIBS = -lrt -lpari -lpthread -lparson -lsha1 -lcriterion

TEST_SRC = $(wildcard *.c) $(wildcard */*.c)
TEST_HDR = $(wildcard */*.h)
TEST_OBJ = $(patsubst %.c,%.o, $(TEST_SRC))
TESTS = $(patsubst %.c,%, $(TEST_SRC))

ECGEN_SRC = $(wildcard ../../src/*/*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

all: unittest

unittest: test_ecgen
	LD_LIBRARY_PATH=../lib/criterion/build DYLD_LIBRARY_PATH=../lib/criterion/build ./test_ecgen --verbose=1 --filter \!\(brainpool_rfc/test_brainpool_rfc_params\)

fulltest: test_ecgen
	LD_LIBRARY_PATH=../lib/criterion/build DYLD_LIBRARY_PATH=../lib/criterion/build ./test_ecgen --verbose=1

test_ecgen: $(TEST_OBJ) $(ECGEN_OBJ)
	$(CC) $(strip $(CPPFLAGS) $(ECGEN_INCLUDES) $(ECGEN_CFLAGS) $(CFLAGS) -o) $@ $^ $(ECGEN_LDFLAGS) $(LDFLAGS) $(ECGEN_LIBS) $(LIBS)

%.o: %.c
	$(CC) $(strip $(CPPFLAGS) $(ECGEN_INCLUDES) $(ECGEN_CFLAGS) $(CFLAGS) -c -o) $@ $<

clean-all: clean
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

.PHONY: all clean-all clean clean-cov format
