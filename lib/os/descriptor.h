#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__
struct SEGMENT_DESCRIPTOR {
    short limit_low;    // Limit[0-15]
    short base_low;     // Base[0-15]
    char base_mid;      // Base[16-23]
    char access_right;  // A, Type, S, DPL, P
    char limit_high;    // Limit[16-19], AVL, reserved, D/B, G
    char base_high;     // Base[24-31]
};

struct GATE_DESCRIPTOR {
    short offset_low;   // Offset[0-15]
    short selector;     // selector
    char dw_count;      //
    char access_right;  // Type, S, DPL, P
    short offset_high;  // Offset[16-31]
};

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#endif
