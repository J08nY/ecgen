####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017,2021 J08nY
#
####

test: ecgen

unittest: unit ecgen

unit:
	./criterion.sh
	+$(MAKE) -C src all

ecgen:
	./ecgen.sh

clean:
	+$(MAKE) -C src clean

clean-all:
	cd lib/criterion && rm -rf build
	+$(MAKE) -C src clean-all

clean-cov:
	+$(MAKE) -C src clean-cov

format:
	+$(MAKE) -C src format
