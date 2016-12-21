
CC=gcc
GP2C=gp2c
CFLAGS=-g -O3 -Wall
GP_CFLAGS=-g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu"
STATIC_CFLAGS=-o points.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -Wl,-shared -Wl,-z,relro points.o -lc -lm -L/usr/lib/x86_64-linux-gnu -lpari
GPFLAGS=-g
LIBS=-lpari -lreadline -ltermcap

GP = points.c sea.c
OBJ = points.o sea.o
DEPS = points.h sea.h

all: ecgen

ecgen: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

gp2c: $(GP)

$(OBJ): $(GP)
	$(CC) -c -o $@ $< $(GP_CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.c: %.gp
	$(GP2C) -o $(basename $@).gp.c $< $(GPFLAGS)
	{ sed -n '/\/\*End of prototype\*\//q;p'; cat >"$@"; } < $(basename $@).gp.c > $(basename $@).h

.PHONY: clean

clean:
	rm *.o
