ENTRY(_start)

SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x40000000;
    __kernel_start = .;
    __text_start = .;
    .text : {
        KEEP(*(.text.boot))
        KEEP(*(.vectors))
        *(.text .text.*)
    }
    . = ALIGN(4K);
    __text_end = .;

    __data_start = .;
    .data : {
        *(.data .data.*)
    }
    . = ALIGN(4K);
    __data_end = .;

    __rodata_start = .;
    .rodata : {
        *(.rodata .rodata.*)
    }
    . = ALIGN(4K);
    __rodata_end = .;

    __bss_start = .;
    .bss : {
        *(.bss .bss.*)
    }
    . = ALIGN(4K);
    __bss_end = .;
    __kernel_end = .;
}
__bss_size = (__bss_end - __bss_start) >> 3;
