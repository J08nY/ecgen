#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#

# Default target first
all:
	+$(MAKE) -C lib all
	+$(MAKE) -C src all

clean:
	+$(MAKE) -C src clean

clean-all:
	+$(MAKE) -C lib clean
	+$(MAKE) -C src clean-all

docs:
	doxygen Doxyfile

help:
	@echo "ecgen, tool for generating Elliptic curve domain parameters"
	@echo
	@echo "Available targets:"
	@echo " - all : builds all"
	@echo " - ecgen : builds the main binary"
	@echo " - docs : generate doxygen docs"
	@echo " - clean : cleans up after a build"
	@echo " - clean-all : cleans all"
	@echo " - format : run clang-format on source files"
	@echo " - help : print this help"

.PHONY: all docs help

ifeq (, $(filter all clean clean-all docs help, $(MAKECMDGOALS)))
# Just pass all targets to a Makefile in src
$(MAKECMDGOALS):
	+$(MAKE) -C src $@
endif
