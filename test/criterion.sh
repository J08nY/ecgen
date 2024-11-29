#!/bin/bash

if [ ! -f "lib/criterion/meson.build" ]; then
	echo "lib/criterion is missing. It is a git submodule from https://github.com/Snaipe/Criterion at 9c01cbe.";
	exit 1;
fi

cd lib/criterion
meson setup build
meson compile -C build
