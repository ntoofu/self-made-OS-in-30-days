void io_hlt();
void write_mem8(int, int);

void OsMain(void) {
    for(int i=0xa0000; i <= 0xaffff; ++i){
        write_mem8(i, 15);
    }

    for(;;) {
        io_hlt();
    }
}
