#ifndef __CURSOR_H__
#define __CURSOR_H__

#define CURSOR_WIDTH 10
#define CURSOR_HEIGHT 16

typedef unsigned char CURSOR_BMP[CURSOR_HEIGHT][CURSOR_WIDTH];

static CURSOR_BMP CURSOR_DEFAULT = {
    "*         ",
    "**        ",
    "*O*       ",
    "*OO*      ",
    "*OOO*     ",
    "*OOOO*    ",
    "*OOOOO*   ",
    "*OOOOOO*  ",
    "*OOOOOOO* ",
    "*OOOO*****",
    "*OOOO*    ",
    "*O**OO*   ",
    "**  *O*   ",
    "*   *OO*  ",
    "     *O*  ",
    "     **   ",
};

#endif


