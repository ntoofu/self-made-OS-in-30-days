    .code16
    .set botpak, 0x00280000
    .set dskcac, 0x00100000
    .set dskcac0, 0x00008000

    # BOOT_INFO
    .set cyls, 0x0ff0
    .set leds, 0x0ff1
    .set vmode, 0x0ff2
    .set scrnx, 0x0ff4
    .set scrny, 0x0ff6
    .set vram, 0x0ff8

    # Reset display mode
    mov $0x13, %al
    mov $0x00, %ah
    int $0x10

    # Store display setting
    movw $8, vmode
    movw $320, scrnx
    movw $200, scrny
    movl $0x000a0000, vram

    # Store keyboard setting
    mov $0x02, %ah
    int $0x16
    movb %al, leds

    # Disable PIC(Programmable Interrupt Controller)
    mov $0xff, %al
    out %al, $0x21
    nop
    out %al, $0xa1

    # Prevent hardware interrupt
    cli

    # configure 'A20 gate' for memory access above 1MB
    call waitkbdout
    mov $0xd1, %al
    out %al, $0x64
    call waitkbdout
    mov $0xdf, %al
    out %al, $0x60
    call waitkbdout

    # turn on protect mode
.arch i486
    lgdt gdtr0
    mov %cr0, %eax
    and $0x7fffffff, %eax
    or $0x00000001, %eax
    mov %eax, %cr0
    jmp pipelineflush

pipelineflush:
    mov $1*8, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # transfer bootpack
    mov $bootpack, %esi
    mov $botpak, %edi
    mov $512*1024/4, %ecx
    call memcpy

    # transfer bootsector
    mov $0x7c00, %esi
    mov $dskcac, %edi
    mov $512/4, %ecx
    call memcpy

    # transfer the rest
    mov $dskcac0 + 512, %esi
    mov $dskcac + 512, %edi
    mov $0, %ecx
    mov cyls, %cl
    imul $512*18*2/4, %ecx
    sub $512/4, %ecx
    call memcpy

    # run bootpack
    mov $0x00310000, %esp
    ljmp $2*8,$0x00000000

# subroutines

waitkbdout:
    in $0x64, %al
    and $0x02, %al
    jnz waitkbdout
    ret

# subroutines(32bit)

memcpy:
    mov (%esi), %eax
    add $4, %esi
    mov %eax, (%edi)
    add $4, %edi
    sub $1, %ecx
    jnz memcpy
    ret

    .balign 16

gdt0:
    .skip 8, 0
    .word 0xffff, 0x0000, 0x9200, 0x00cf
    .word 0xffff, 0x0000, 0x9a28, 0x0047
    .word 0

gdtr0:
    .word 8*3-1
    .long gdt0
    .balign 16

bootpack:
