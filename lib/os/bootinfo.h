#ifndef __BOOTINFO_H__
#define __BOOTINFO_H__

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
};

#endif
