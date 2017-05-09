# BOOT_INFO
.set cyls, 0x0ff0
.set leds, 0x0ff1
.set vmode, 0x0ff2
.set scrnx, 0x0ff4
.set scrny, 0x0ff6
.set vram, 0x0ff8

    # reset display
    mov $0x13, %al
    mov $0x00, %ah
    int $0x10

    # save display mode
    movb $8, vmode
    movl $320, scrnx
    movl $200, scrny
    movw $0x000a0000, vram

    # retrieve keyboard state
    mov $0x02, %ah
    int $0x16
    mov %al, leds

fin:
    hlt
    jmp fin
