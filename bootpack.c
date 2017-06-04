void io_hlt();

void OsMain(void) {
    for(int i=0xa0000; i <= 0xaffff; ++i){
        *((char*)i) = i & 0x0f;
    }

    for(;;) {
        io_hlt();
    }
}
