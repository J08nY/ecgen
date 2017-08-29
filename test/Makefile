####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

test: ecgen econvert

unittest: unit ecgen econvert

unit:
	cd lib/criterion && mkdir -p build && cd build && cmake .. && cmake --build .
	+$(MAKE) -C src all

ecgen:
	./ecgen.sh

econvert:
	./econvert.sh

clean-all:
	cd lib/criterion && rm -r build
	+$(MAKE) -C src clean-all