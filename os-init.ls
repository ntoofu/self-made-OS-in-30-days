OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

SECTIONS {
. = 0xc400;
.text : { *(.text) }
}
