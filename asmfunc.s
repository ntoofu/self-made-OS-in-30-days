.file "asmfunc.s"

.text

# void io_hlt()
.global io_hlt
.type io_hlt, @function
io_hlt:
    hlt
    ret

# void write_mem8(int addr, int data)
.global write_mem8
.type write_mem8, @function
write_mem8:
    movl 4(%esp), %ecx
    movb 8(%esp), %al
    mov %al, (%ecx)
    ret

