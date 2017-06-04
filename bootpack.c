#include "asmfunc.h"
#include "color.h"

void OsMain(void) {
    char* p = 0xa0000;

    init_palette();

    for(int i=0x0000; i <= 0xffff; ++i){
        p[i] = i & 0x0f;
    }

    boxfill8(p, 320, COL8_RED, 20, 20, 120, 120);
    boxfill8(p, 320, COL8_GREEN, 70, 50, 170, 120);
    boxfill8(p, 320, COL8_BLUE, 120, 80, 220, 120);

    for(;;) {
        io_hlt();
    }
}

void init_palette() {
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0xff, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84,
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb) {
    int eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for(int i=start; i<=end; ++i) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb +=3;
    }
    io_store_eflags(eflags);
    return;
}

void boxfill8(unsigned char *vram, int vram_xsize, unsigned char color, int x0, int y0, int x1, int y1) {
    int pos = x0 + y0 * vram_xsize;
    const int intv = vram_xsize - ( x1 - x0 + 1 );
    for(int y=y0; y<=y1; ++y) {
        for(int x=x0; x<=x1; ++x) {
            vram[pos] = color;
            ++pos;
        }
        pos += intv;
    }
    return;
}

