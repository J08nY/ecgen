#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#

# Default target first
all:
	+$(MAKE) -C lib all
	+$(MAKE) -C src all

clean:
	+$(MAKE) -C lib clean
	+$(MAKE) -C src clean

docs:
	doxygen Doxyfile

help:
	@echo "ecgen, tool for generating Elliptic curve domain parameters"
	@echo
	@echo "Available targets:"
	@echo " - all : builds all"
	@echo " - ecgen : builds the main binary"
	@echo " - gp2c : generates the .c and .h files from gp code"
	@echo " - docs : generate doxygen docs"
	@echo " - clean : cleans up after a build"
	@echo " - clean-gp : cleans up after gp2c generation"
	@echo " - clean-all : cleans all"
	@echo " - format : run clang-format on source files"
	@echo " - help : print this help"

.PHONY: all docs help

ifeq (, $(filter all clean docs help, $(MAKECMDGOALS)))
# Just pass all targets to a Makefile in src
$(MAKECMDGOALS):
	+$(MAKE) -C src $@
endif
