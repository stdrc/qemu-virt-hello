#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

main() {
	rm -rf build && mkdir build

	echo "compiling kernel image..."
	cd build

	cmake -G Ninja ..
	ninja

	exit 0
}

main $@
