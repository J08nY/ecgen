CC=gcc
CCFLAGS=-Wall
LDFLAGS=

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

TARGET=sha1
A=libsha1.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(A): $(OBJECTS)
	ar rcs $(A) $(OBJECTS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<


clean:
	rm -f *.o *.a $(TARGET)

.PHONY: clean