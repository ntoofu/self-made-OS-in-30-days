IPL=hello-os.img
LINKER_SCRIPT=binary.ls

.PHONY: all
all: $(IPL:img=o)
	ld $(IPL:img=o) -T $(LINKER_SCRIPT) -o $(IPL)

.s.o:
	as --32 $< -o $*.o
