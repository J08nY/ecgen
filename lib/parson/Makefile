CC ?= gcc
CCFLAGS ?= -Wall
LDFLAGS =

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = parson
A = libparson.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(A): $(OBJECTS)
	ar rcs $(A) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.a $(TARGET)

.PHONY: clean
