CC ?= gcc
CFLAGS ?= -Wall
LDFLAGS =

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = sha1
A = libsha1.a

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
