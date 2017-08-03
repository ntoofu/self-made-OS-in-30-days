#include "bootinfo.h"
#include "interrupt.h"
#include "../graphic/display.h"
#include "../graphic/font.h"
#include "../os/asmfunc.h"
#include "../../constants.h"

void init_pic() {
    // mask all IRQ
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);
    // configure registers
    io_out8(PIC0_ICW1, 0x11); // edge trigger
    io_out8(PIC0_ICW2, 0x20); // use INT 0x20-0x27
    io_out8(PIC0_ICW3, 1<<2); // PIC1 is connected to IRQ2
    io_out8(PIC0_ICW4, 0x01); // non-buffer
    io_out8(PIC1_ICW1, 0x11); // edge trigger
    io_out8(PIC1_ICW2, 0x28); // use INT 0x20-0x27
    io_out8(PIC1_ICW3, 2   ); // PIC1 is connected to IRQ2
    io_out8(PIC1_ICW4, 0x01); // non-buffer
    // unmask
    io_out8(PIC0_IMR, 0xfb);  // accept only IRQ2
    io_out8(PIC1_IMR, 0xff);
}

void int_handler_kbd(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO*) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_WHITE, 0, 0, 255, 15);
    putstr8(binfo->vram, binfo->scrnx, COL8_BLACK, 0, 0, "INT 0x21", FONT_OSASK);
    for(;;) {
        io_hlt();
    }
}
// asm_int_handler_kbd
__asm__ (
    ".global asm_int_handler_kbd\n\t"
    "asm_int_handler_kbd:       \n\t"
    "   push %es                \n\t"
    "   push %ds                \n\t"
    "   pusha                   \n\t"
    "   mov %esp, %eax          \n\t"
    "   push %eax               \n\t"
    "   mov %ss, %ax            \n\t"
    "   mov %ax, %ds            \n\t"
    "   mov %ax, %es            \n\t"
    "   call int_handler_kbd    \n\t"
    "   pop %eax                \n\t"
    "   popa                    \n\t"
    "   push %ds                \n\t"
    "   push %es                \n\t"
    "   iret                    \n\t"
);

void int_handler_mc(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO*) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_WHITE, 0, 0, 255, 15);
    putstr8(binfo->vram, binfo->scrnx, COL8_BLACK, 0, 0, "INT 0x2c", FONT_OSASK);
    for(;;) {
        io_hlt();
    }
}

__asm__ (
    ".global asm_int_handler_mc \n\t"
    "asm_int_handler_mc:        \n\t"
    "   push %es                \n\t"
    "   push %ds                \n\t"
    "   pusha                   \n\t"
    "   mov %esp, %eax          \n\t"
    "   push %eax               \n\t"
    "   mov %ss, %ax            \n\t"
    "   mov %ax, %ds            \n\t"
    "   mov %ax, %es            \n\t"
    "   call int_handler_mc     \n\t"
    "   pop %eax                \n\t"
    "   popa                    \n\t"
    "   push %ds                \n\t"
    "   push %es                \n\t"
    "   iret                    \n\t"
);

