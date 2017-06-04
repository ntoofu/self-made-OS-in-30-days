void io_hlt();
void io_cli();
void io_sti();
int io_load_eflags();
void io_store_eflags(int eflags);
void io_out8(int port, int data);
void write_mem8(int addr, int data);
