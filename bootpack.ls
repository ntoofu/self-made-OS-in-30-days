OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

SECTIONS {
.text 0x00 : { *(.text) }

.data 0x280000 + SIZEOF(.text):
    AT ( LOADADDR(.text) + SIZEOF(.text) ) { *(.data) }

.rodata :
    AT ( LOADADDR(.data) + SIZEOF(.data) ) { *(.rodata*) }

.bss :
    AT ( LOADADDR(.rodata) + SIZEOF(.rodata) ) { *(.bss) }
}
