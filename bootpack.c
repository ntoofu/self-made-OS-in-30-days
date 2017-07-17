#include "lib/os/asmfunc.h"
#include "lib/graphic/color.h"
#include "lib/graphic/font.h"
#include "lib/graphic/cursor.h"
#include "lib/graphic/display.h"
#include "lib/os/descriptor.h"
#include "lib/util/sprintf.h"

void init_gdt();
void init_idt();

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
};

void OsMain(void) {
    struct BOOTINFO *binfo;
    binfo = (struct BOOTINFO *) 0x0ff0;

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

    for(;;) {
        io_hlt();
    }
}

void init_gdt() {
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
    for(int i=0; i<8192; ++i) {
        set_segmdesc(gdt+i, 0, 0, 0);
    }
    set_segmdesc(gdt+1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt+1, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);
}

void init_idt() {
    struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *) 0x0026f800;
    for(int i=0; i<256; ++i) {
        set_gatedesc(idt+i, 0, 0, 0);
    }
    load_idtr(0x07ff, 0x0026f800);
}

