.file "asmfunc.s"

.text

# void io_hlt()
.global io_hlt
.type io_hlt, @function
io_hlt:
    hlt
    ret

# void io_cli();
.global io_cli
.type io_cli, @function
io_cli:
    cli
    ret

# void io_sti();
.global io_sti
.type io_sti, @function
io_sti:
    sti
    ret

# int io_load_eflags();
.global io_load_eflags
.type io_load_eflags, @function
io_load_eflags:
    pushfl
    pop %eax
    ret

# void io_store_eflags(int eflags);
.global io_store_eflags
.type io_store_eflags, @function
io_store_eflags:
    mov 4(%esp), %eax
    push %eax
    popfl
    ret

# void io_out8(int port, int data);
.global io_out8
.type io_out8, @function
io_out8:
    mov 4(%esp), %edx
    mov 8(%esp), %al
    out %al, %dx
    ret

# void write_mem8(int addr, int data)
.global write_mem8
.type write_mem8, @function
write_mem8:
    movl 4(%esp), %ecx
    movb 8(%esp), %al
    mov %al, (%ecx)
    ret

