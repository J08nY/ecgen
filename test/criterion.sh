#!/bin/bash

if [ ! -f "lib/criterion/CMakeLists.txt" ]; then
	echo "lib/criterion is missing. It is a git submodule from https://github.com/Snaipe/Criterion at a64b860.";
	exit 1;
fi

cd lib/criterion
mkdir -p build
cd build
cmake ..  >/dev/null
cmake --build . >/dev/null
