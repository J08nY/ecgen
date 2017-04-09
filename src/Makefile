####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

CC ?= gcc

CFLAGS = -Wall
DEBUG ?= 1
ifeq (DEBUG, 1)
    CFLAGS += -g -DDEBUG -v
else
    CFLAGS += -O3 -DNDEBUG
endif
LDFLAGS = -L../lib/parson -L../lib/sha1 -L../lib/pari

INCLUDES = -I. -I../lib
LIBS = -lrt -lpthread -lparson -lsha1 -lpari

####

VPATH = cm:invalid:io:exhaustive:math

ECGEN_SRC = ecgen.c $(wildcard */*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

ECONVERT_SRC = econvert.c
ECONVERT_OBJ = $(patsubst %.c,%.o, $(ECONVERT_SRC))

SRC = $(wildcard *.c) $(wildcard */*.c)
HDR = $(wildcard */*.h)

####

all: ecgen econvert

ecgen: ecgen.o $(ECGEN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)
	mv ecgen ..

econvert: econvert.o $(ECONVERT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)
	mv econvert ..

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

####

clean-all: clean
	rm -f ../ecgen
	rm -f ../econvert

clean:
	find . -type f -name '*.o' -exec rm {} +

format:
	clang-format -i $(SRC)
	clang-format -i $(HDR)

.PHONY: all clean-all clean format
