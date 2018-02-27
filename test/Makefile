####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

test: ecgen

unittest: unit ecgen

unit:
	cd lib/criterion && mkdir -p build && cd build && cmake ..  >/dev/null && cmake --build . >/dev/null
	+$(MAKE) -C src all

ecgen:
	./ecgen.sh

clean:
	+$(MAKE) -C src clean

clean-all:
	cd lib/criterion && rm -rf build
	+$(MAKE) -C src clean-all

format:
	+$(MAKE) -C src format