####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017-2021 J08nY
#
####

CC ?= gcc

CFLAGS = -Wall
DEBUG ?= 0
TEST ?= 0
FAST ?= 0
STATIC ?= 0

ifeq ($(DEBUG), 1)
    ECGEN_CFLAGS = -DDEBUG -g -Og -Werror -pedantic
else ifeq ($(TEST), 1)
	ECGEN_CFLAGS = -DNDEBUG --coverage -g -O0
else ifeq ($(FAST), 1)
    ifeq ($(CC), gcc)
        ECGEN_CFLAGS = -DNDEBUG -O3 -march=native -mtune=native -pipe -flto
    else
        ECGEN_CFLAGS = -DNDEBUG -O3
    endif
else
    ECGEN_CFLAGS = -DNDEBUG -O2
endif

ECGEN_LDFLAGS = -L../lib/parson -L../lib/sha1 -L../lib/pari
ifeq ($(STATIC), 1)
	ECGEN_LIBS = -lrt -Wl,-Bstatic -lpari -Wl,-Bdynamic -lpthread -lparson -lsha1 -lm -lgmp -ldl
else
	ECGEN_LIBS = -lrt -lpari -lpthread -lparson -lsha1
endif

ECGEN_INCLUDES = -I. -I../lib


GIT_COMMIT = $(shell git rev-parse --short HEAD 2>/dev/null)
ifneq ($(GIT_COMMIT),)
	ECGEN_CFLAGS += -DGIT_COMMIT=\"$(GIT_COMMIT)\"
endif

####

ECGEN_SRC = ecgen.c $(wildcard */*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

SRC = $(wildcard *.c) $(wildcard */*.c)
HDR = $(wildcard */*.h)

####

all: ecgen

ecgen: ecgen.o $(ECGEN_OBJ)
	$(CC) $(strip $(CPPFLAGS) $(ECGEN_INCLUDES) $(ECGEN_CFLAGS) $(CFLAGS) -o) $@ $^ $(ECGEN_LDFLAGS) $(LDFLAGS) $(ECGEN_LIBS) $(LIBS)
	mv ecgen ..

%.o: %.c
	$(CC) $(strip $(CPPFLAGS) $(ECGEN_INCLUDES) $(ECGEN_CFLAGS) $(CFLAGS) -c -o) $@ $<

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
