#include "descriptor.h"

// void load_gdtr(int limit, int addr)
__asm__ (
    ".global load_gdtr      \n\t"
    "load_gdtr:             \n\t"
    "    mov 4(%esp), %ax   \n\t"
    "    mov %ax, 6(%esp)   \n\t"
    "    lgdt 6(%esp)       \n\t"
    "    ret                \n\t"
);

// void load_idtr(int limit, int addr)
__asm__ (
    ".global load_idtr      \n\t"
    "load_idtr:             \n\t"
    "   mov 4(%esp), %ax    \n\t"
    "   mov %ax, 6(%esp)    \n\t"
    "   lidt 6(%esp)        \n\t"
);

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar) {
    if(limit > 0xffffff) {
        ar |= 0x8000;
        limit /= 0x1000;
    }
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->base_high = (base >> 24) & 0xff;
    sd->limit_low = limit & 0xffff;
    sd->limit_high = ( (limit >> 16) & 0x0f ) | ( (ar >> 8) & 0xf0 );
    sd->access_right = ar & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar) {
    gd->offset_low = offset & 0xffff;
    gd->offset_high = (offset >> 16) & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    return;
}
