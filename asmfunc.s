.file "asmfunc.s"

.text

# void io_hlt()
.global _io_hlt
_io_hlt:
    hlt
    ret

# void write_mem8(int addr, int data)
.global _write_mem8:
    movl 4(%esp), %ecx
    movb 8(%esp), %al
    mov %al, (%ecx)


