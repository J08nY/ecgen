CC ?= gcc

DEBUG ?= 0
TEST ?= 0
FAST ?= 0
ifeq ($(DEBUG), 1)
    ECGEN_CFLAGS = -DDEBUG -g -Og -Werror -pedantic
else ifeq ($(TEST), 1)
	ECGEN_CFLAGS = -DNDEBUG --coverage -g -O0
else ifeq ($(FAST), 1)
    ifeq ($(CC), gcc)
        ECGEN_CFLAGS = -DNDEBUG -O3 -march=native -pipe
    else
        ECGEN_CFLAGS = -DNDEBUG -O3
    endif
else
    ECGEN_CFLAGS = -DNDEBUG -O2
endif
export

all:
	$(MAKE) -C parson libparson.a
	$(MAKE) -C sha1 libsha1.a

clean:
	$(MAKE) -C parson clean
	$(MAKE) -C sha1 clean
