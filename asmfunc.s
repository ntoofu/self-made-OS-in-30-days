.file "asmfunc.s"

.text

# void io_hlt()
.global _io_hlt
_io_hlt:
    hlt
    ret
