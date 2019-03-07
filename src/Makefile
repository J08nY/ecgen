####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017-2018 J08nY
#
####

CC ?= gcc

CFLAGS = -Wall
DEBUG ?= 0
TEST ?= 0
FAST ?= 0
STATIC ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG -g -Og -Werror -pedantic
else ifeq ($(TEST), 1)
	CFLAGS += -DNDEBUG --coverage -g -O0
else ifeq ($(FAST), 1)
    ifeq ($(CC), gcc)
        CFLAGS += -DNDEBUG -O3 -march=native -mtune=native -pipe
    else
        CFLAGS += -DNDEBUG -O3
    endif
else
    CFLAGS += -DNDEBUG -O2
endif

LDFLAGS = -L../lib/parson -L../lib/sha1 -L../lib/pari
ifeq ($(STATIC), 1)
	LIBS = -lrt -Wl,-Bstatic -lpari -Wl,-Bdynamic -lpthread -lparson -lsha1 -lm -lgmp -ldl
else
	LIBS = -lrt -lpari -lpthread -lparson -lsha1
endif

INCLUDES = -I. -I../lib


GIT_COMMIT = $(shell git rev-parse --short HEAD)
ifneq ($(GIT_COMMIT),)
	CFLAGS += -DGIT_COMMIT=\"$(GIT_COMMIT)\"
endif

####

ECGEN_SRC = ecgen.c $(wildcard */*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

SRC = $(wildcard *.c) $(wildcard */*.c)
HDR = $(wildcard */*.h)

####

all: ecgen

ecgen: ecgen.o $(ECGEN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)
	mv ecgen ..

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

####

clean-all: clean
	rm -f ../ecgen

clean:
	find . -type f -name '*.o' -exec rm {} +

clean-cov:
	find . -type f -name '*.gcda' -exec rm {} +
	find . -type f -name '*.gcno' -exec rm {} +
	find . -type f -name '*.gcov' -exec rm {} +

format:
	clang-format -i $(SRC)
	clang-format -i $(HDR)

.PHONY: all clean-all clean clean-cov format
