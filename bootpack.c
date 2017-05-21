void io_hlt();

void OsMain(void) {
    for(int i=0xa0000; i <= 0xaffff; ++i){
        write_mem(i, 15);
    }

    for(;;) {
        io_hlt();
    }
}
