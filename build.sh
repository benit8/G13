#!/bin/sh
set -e

bin="g13d"

# does `./.build` directory exists ?
if [ ! -d ./.build ]; then
	mkdir ./.build
fi

clear
cd ./.build
cmake .. -G "Unix Makefiles"
cmake --build . -- -j $(nproc)

# we still in ./.build
cp "./$bin" ../