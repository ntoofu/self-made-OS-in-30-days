#include "constants.h"
#include "lib/graphic/color.h"
#include "lib/graphic/font.h"
#include "lib/graphic/cursor.h"
#include "lib/graphic/display.h"
#include "lib/os/asmfunc.h"
#include "lib/os/bootinfo.h"
#include "lib/os/descriptor.h"
#include "lib/os/interrupt.h"
#include "lib/util/sprintf.h"

void init_gdt();
void init_idt();

void OsMain(void) {
    struct BOOTINFO *binfo;
    binfo = (struct BOOTINFO *) ADR_BOOTINFO;

    init_gdt();
    init_idt();
    init_pic();
    io_sti();

    init_palette();

    for(int i=0x0000; i <= 0xffff; ++i){
        (binfo->vram)[i] = i & 0x0f;
    }

    boxfill8(binfo->vram, binfo->scrnx, COL8_RED, 20, 20, 120, 120);
    boxfill8(binfo->vram, binfo->scrnx, COL8_GREEN, 70, 50, 170, 120);
    boxfill8(binfo->vram, binfo->scrnx, COL8_BLUE, 120, 80, 220, 120);

    putstr8(binfo->vram, binfo->scrnx, COL8_BLACK, 20, 20, "Hello World!!", FONT_OSASK);
    char str[40];
    sprintf(str, "scrnx = %d", binfo->scrnx);
    putstr8(binfo->vram, binfo->scrnx, COL8_BLACK, 20, 70, str, FONT_OSASK);

    CURSOR_BMP cursor_bmp;
    init_mouse_cursor8(cursor_bmp, CURSOR_DEFAULT, COL8_GREY);
    putbitmap8_8(binfo->vram, binfo->scrnx, CURSOR_WIDTH, CURSOR_HEIGHT, 150, 80, (char*)cursor_bmp, CURSOR_WIDTH);

    io_out8(PIC0_IMR, 0b11111001);
    io_out8(PIC1_IMR, 0b11101111);

    for(;;) {
        io_hlt();
    }
}

void init_gdt() {
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
    for(int i=0; i<8192; ++i) {
        set_segmdesc(gdt+i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
    load_gdtr(LIMIT_GDT, ADR_GDT);
}

void init_idt() {
    struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *) 0x0026f800;
    for(int i=0; i<256; ++i) {
        set_gatedesc(idt+i, 0, 0, 0);
    }
    load_idtr(LIMIT_IDT, ADR_IDT);
    set_gatedesc(idt + 0x21, (int) asm_int_handler_kbd, 2 << 3, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int) asm_int_handler_mc, 2 << 3, AR_INTGATE32);
}

