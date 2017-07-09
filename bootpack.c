#include "asmfunc.h"
#include "color.h"
#include "font.h"
#include "cursor.h"
#include "lib/sprintf.h"

void init_palette();
void set_palette();
void boxfill8(unsigned char *vram, int vram_xsize, unsigned char color, int x0, int y0, int x1, int y1);
void putfont8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char c, FONT f);
void putstr8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char* c, FONT f);
void init_mouse_cursor8(CURSOR_BMP bmp_buf, const CURSOR_BMP cursor_bmp, char bgcolor);
void putbitmap8_8(char *vram, int vram_xsize, int xsize, int ysize, int x, int y, const char *bmp, int bmp_xsize);

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

void putfont8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char c, FONT f) {
    const int top_pos = x + y * vram_xsize;
    for(int fy=0; fy<16; ++fy) {
        const unsigned char c_line = f[c][fy];
        for(int fx=0; fx<8; ++fx) {
            if ( c_line & (1 << (7-fx)) ) {
                int pos = top_pos + fy * vram_xsize + fx;
                vram[pos] = color;
            }
        }
    }
}


void putstr8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char* c, FONT f) {
    for(int cx=x; *c != 0x00; ++c, cx+=8) {
        putfont8(vram, vram_xsize, color, cx, y, *c, f);
    }
}

void init_mouse_cursor8(CURSOR_BMP bmp_buf, const CURSOR_BMP cursor_bmp, char bgcolor) {
    char* bufp = (char*) bmp_buf;
    for(int y=0; y<CURSOR_HEIGHT; ++y) {
        for(int x=0; x<CURSOR_WIDTH; ++x) {
            char pix = cursor_bmp[y][x];
            switch(pix) {
                case '*':
                    *bufp = COL8_BLACK;
                    break;
                case 'O':
                    *bufp = COL8_WHITE;
                    break;
                case ' ':
                    *bufp = bgcolor;
                    break;
            }
            ++bufp;
        }
    }
    return;
}

void putbitmap8_8(char *vram, int vram_xsize, int xsize, int ysize, int x, int y, const char *bmp, int bmp_xsize) {
    for(int y_rel=0; y_rel<ysize; ++y_rel) {
        for(int x_rel=0; x_rel<xsize; ++x_rel) {
            vram[(y+y_rel) * vram_xsize + (x+x_rel)] = bmp[y_rel * bmp_xsize + x_rel];
        }
    }
    return;
}
