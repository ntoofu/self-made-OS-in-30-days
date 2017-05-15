void io_hlt();

void OsMain(void) {
fin:
    io_hlt();
    goto fin;
}
