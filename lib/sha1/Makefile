CC ?= gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = sha1
A = libsha1.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(strip $(CPPFLAGS) $(CFLAGS) -o) $@ $^ $(LDFLAGS) $(LIBS)

$(A): $(OBJECTS)
	ar rcs $(A) $(OBJECTS)

%.o: %.c
	$(CC) $(strip $(CPPFLAGS) $(CFLAGS) -c) $<

clean:
	rm -f *.o *.a $(TARGET)

.PHONY: clean
