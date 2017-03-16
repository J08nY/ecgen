####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

CC ?= gcc
GP2C = gp2c

CFLAGS = -O3 -Wall
LDFLAGS = -L../lib/parson -L../lib/sha1

GP_CFLAGS = -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC
GPFLAGS = -g -i4

INCLUDES = -I. -I../lib
LIBS = -lrt -lpari -lparson

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
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.h %.c: %.gp
	$(GP2C) $(GPFLAGS) gp/$*.gp  2>/dev/null | clang-format | awk 'BEGIN{print "#ifndef GP_H" > "gp.h"; print "#define GP_H" >> "gp.h";} { if(found) print >> "gp.c"; else print >> "gp.h"} /End of prototype/{found=1; print "#include \"gp.h\"" > "gp.c"; print "#endif //GP_H" >> "gp.h"}'

####

clean-all: clean clean-gp

clean:
	find . -type f -name '*.o' -exec rm {} +

clean-gp:
	rm -f $(GPH)
	rm -f $(GPC)

format:
	clang-format -i $(SRC)
	clang-format -i $(HDR)

.PHONY: all gp2c clean-all clean clean-gp format
