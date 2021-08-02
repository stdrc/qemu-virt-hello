#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

main() {
	rm -rf build && mkdir build

	echo "compiling kernel image..."
	cd build

	cmake -G "Unix Makefiles" ..
	make

	exit 0
}

main $@
