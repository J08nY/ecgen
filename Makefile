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
	+$(MAKE) -C test clean

clean-cov:
	+$(MAKE) -C src clean-cov
	+$(MAKE) -C test clean-cov
	rm -rf *.gcov

clean-all:
	+$(MAKE) -C lib clean
	+$(MAKE) -C src clean-all
	+$(MAKE) -C test clean-all
	rm -rf docs/html docs/latex

format:
	+$(MAKE) -C src format
	+$(MAKE) -C test format

docs:
	doxygen Doxyfile

test: all
	+$(MAKE) -C test test

unittest: all
	+$(MAKE) -C test unittest

help:
	@echo "ecgen, tool for generating Elliptic curve domain parameters"
	@echo
	@echo "Available targets:"
	@echo " - all : builds all"
	@echo " - ecgen : builds the main binary"
	@echo " - docs : generate doxygen docs"
	@echo " - test : test the main binary"
	@echo " - unittest : "
	@echo " - clean : cleans up after a build"
	@echo " - clean-cov : cleans up coverage files"
	@echo " - clean-all : cleans all"
	@echo " - format : run clang-format on source files"
	@echo " - help : print this help"
	@echo
	@echo "Available variables:"
	@echo " - CC : The compiler to use"
	@echo " - DEBUG : Whether to enable DEBUG build = default 0"
	@echo " - TEST : Whether to enable TEST build(code coverage) = default 0"
	@echo " - FAST : Whether to enable additional optimization(gcc only) = default 0"
	@echo " - STATIC : Whether to link PARI statically = default 0"

.PHONY: all clean clean-all docs test unittest help

ifeq (, $(filter all clean clean-cov clean-all format docs test unittest help, $(MAKECMDGOALS)))
# Just pass all targets to a Makefile in src
$(MAKECMDGOALS):
	+$(MAKE) -C src $@
endif
