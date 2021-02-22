.PHONY: build kernel run format

build: kernel

kernel:
	$(Q)./build.sh

run:
	$(Q)qemu-system-aarch64 \
		-machine virt,gic-version=2,secure=off,virtualization=off \
		-cpu cortex-a57 -smp 1 -m 2G -kernel build/kernel.img -nographic

format:
	$(Q)find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i -style=file
