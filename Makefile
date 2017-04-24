LINKER_SCRIPT=binary.ls

.PHONY: all
all: ipl.img
	gzip -cd hello-os_disk.img.gz > hello-os.img
	dd conv=notrunc if=ipl.img of=hello-os.img bs=512 count=1

ipl.img: hello-os.o
	ld hello-os.o -T $(LINKER_SCRIPT) -o $@

.s.o:
	as --32 $< -o $*.o
