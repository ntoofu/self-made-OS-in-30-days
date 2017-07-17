#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "color.h"
#include "font.h"
#include "cursor.h"

void init_palette();
void set_palette();
void boxfill8(unsigned char *vram, int vram_xsize, unsigned char color, int x0, int y0, int x1, int y1);
void putfont8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char c, FONT f);
void putstr8(unsigned char *vram, int vram_xsize, unsigned char color, int x, int y, unsigned char* c, FONT f);
void init_mouse_cursor8(CURSOR_BMP bmp_buf, const CURSOR_BMP cursor_bmp, char bgcolor);
void putbitmap8_8(char *vram, int vram_xsize, int xsize, int ysize, int x, int y, const char *bmp, int bmp_xsize);

#endif
