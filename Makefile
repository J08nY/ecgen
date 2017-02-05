#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#

# Default target first
all:
	+$(MAKE) -C src all

# Just pass all targets to a Makefile in src
$(MAKECMDGOALS):
	+$(MAKE) -C src $@

