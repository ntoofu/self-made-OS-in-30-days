.code16
# FAT12 floppy disk

base:
    .byte 0xeb, 0x4e, 0x90
    .ascii "HELLOIPL"
    .word 512 # bytes per sector
    .byte 1 # sectors per cluster
    .word 1 # number of reserced sectors
    .byte 2 # number of FATs
    .word 224 # maximum number of root directory entries
    .word 2880 # total sector count
    .byte 0xf0 # ignored (media type?)
    .word 9 # sectors per FAT
    .word 18 # sectors per track
    .word 2 # number of heads
    .int 0 # ignored
    .int 2880 # total sector cound for FAT32
    .byte 0, 0 # ignored
    .byte 0x29 # boot signature
    .int 0xffffffff #volume id
    .ascii "HELLO-OS   " # volume label (11bytes)
    .ascii "FAT12   " # File system type (8bytes)
    .skip 18, 0 # ignored; rest of boot sector

# Program
entry:
    mov $0, %ax
    mov %ax, %ss
    mov $base, %sp
    mov %ax, %ds
    mov %ax, %es

    movw $msg, %si      # si = msg; // char[] msg;

putloop:                # putloop:
    movb (%si), %al     # al = *si;
    add $0x01, %si      # ++si;
    cmpb $0x00, %al     # if (al == 0)
    je fin              #     goto fin;
    movb $0x0e, %ah
    movw $0x000f, %bx
    int $0x10
    jmp putloop         # goto putloop;

fin:
    hlt
    jmp fin

# message
msg:
    .byte 0x0a, 0x0a
    .ascii "Hello, world"
    .byte 0x0a
    .byte 0
    .org 0x1fe, 0

    .byte 0x55, 0xaa
