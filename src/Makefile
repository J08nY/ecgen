####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

CC=gcc
GP2C=gp2c

CFLAGS=-O3 -Wall
LDFLAGS=-L../lib

GP_CFLAGS=-O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC
GPFLAGS=-g -i4

INCLUDES=-I. -I../lib -Icm -Iinvalid -Iio -Irandom -Iexhaustive
LIBS=-lpari -lparson

####

VPATH = cm:invalid:io:exhaustive:math

GP =
GPC = $(addsuffix .c, $(GP))
GPO = $(addsuffix .o, $(GP))
GPH = $(addsuffix .h, $(GP))

SRC = ecgen.c $(wildcard */*.c)
OBJ = $(patsubst %.c,%.o, $(SRC))
HDR = $(wildcard *.h) $(wildcard */*.h)

####

all: ecgen

ecgen: ecgen.o $(GPO) $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)
	mv ecgen ..

gp2c: $(GPC) $(GPH)

$(GPO): $(GPC) $(GPH)
	$(CC) $(GP_CFLAGS) $(INCLUDES) -c -o $@ $< $(LDFLAGS) $(LIBS)

%.o: %.c $(GPH)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $< $(LDFLAGS) $(LIBS)

%.h %.c: %.gp
	$(GP2C) $(GPFLAGS) gp/$*.gp  2>/dev/null | clang-format | awk 'BEGIN{print "#ifndef GP_H" > "gp.h"; print "#define GP_H" >> "gp.h";} { if(found) print >> "gp.c"; else print >> "gp.h"} /End of prototype/{found=1; print "#include \"gp.h\"" > "gp.c"; print "#endif //GP_H" >> "gp.h"}'

####

clean-all: clean clean-gp

clean:
	rm -f ecgen
	find . -type f -name '*.o' -exec rm {} +

clean-gp:
	rm -f $(GPH)
	rm -f $(GPC)

help:
	@echo "ecgen, tool for generating Elliptic curve domain parameters"
	@echo
	@echo "Available targets:"
	@echo " - all : builds all"
	@echo " - ecgen : builds the main binary"
	@echo " - gp2c : generates the .c and .h files from gp code"
	@echo " - clean : cleans up after a build"
	@echo " - clean-gp : cleans up after gp2c generation"
	@echo " - clean-all : cleans all"
	@echo " - format : run clang-format on source files"

format:
	clang-format -i $(SRC)
	clang-format -i $(HDR)

.PHONY: all gp2c clean-all clean clean-gp help format
