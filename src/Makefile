####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

CC ?= gcc

CFLAGS = -O3 -Wall
LDFLAGS = -L../lib/parson -L../lib/sha1

INCLUDES = -I. -I../lib
LIBS = -lrt -lpari -lparson

####

VPATH = cm:invalid:io:exhaustive:math

ECGEN_SRC = ecgen.c $(wildcard */*.c)
ECGEN_OBJ = $(patsubst %.c,%.o, $(ECGEN_SRC))

ECONVERT_SRC = econvert.c $(wildcard */*.c)
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
	rm ../ecgen
	rm ../econvert

clean:
	find . -type f -name '*.o' -exec rm {} +

format:
	clang-format -i $(SRC)
	clang-format -i $(HDR)

.PHONY: all gp2c clean-all clean clean-gp format
