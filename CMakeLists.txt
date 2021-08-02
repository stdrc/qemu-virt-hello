cmake_minimum_required(VERSION 3.14)

set(CROSS_COMPILE "aarch64-linux-gnu-")
set(CMAKE_LINKER "${CROSS_COMPILE}ld")
set(CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_ASM_LINK_EXECUTABLE
    "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_COMPILER "${CROSS_COMPILE}gcc")
set(CMAKE_ASM_COMPILER "${CROSS_COMPILE}gcc")
set(CMAKE_OBJCOPY "${CROSS_COMPILE}objcopy")
set(CMAKE_OBJDUMP "${CROSS_COMPILE}objdump")

project(gic-vm-test C ASM)

add_compile_options(
    -O0
    -g
    -Wall
    -fPIC
    -nostdlib
    -nostartfiles
    -ffreestanding
    -Werror
    -nostdinc
    -march=armv8-a+nofp
)
add_compile_definitions($<$<COMPILE_LANGUAGE:ASM>:__ASM__>)
add_link_options(-T ${CMAKE_CURRENT_LIST_DIR}/kernel.ld)

include_directories(kernel)

set(KERNEL_IMG kernel.img)
set(KERNEL_BIN kernel.bin)
set(KERNEL_ASM kernel.asm)

set(KERNEL_SOURCES
    kernel/start.S
    kernel/tools.S
    kernel/vector.S
    kernel/main.c
    kernel/irq.S
    kernel/irq.c
    kernel/irq_entry.c
    kernel/gic.c
    kernel/timer.c
    kernel/uart.c
    kernel/printf.c)
add_executable(${KERNEL_IMG} ${KERNEL_SOURCES})

add_custom_command(
    TARGET ${KERNEL_IMG}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${KERNEL_IMG} ${KERNEL_BIN}
)

add_custom_command(
    TARGET ${KERNEL_IMG}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} ARGS -D ${KERNEL_IMG} > ${KERNEL_ASM}
)
