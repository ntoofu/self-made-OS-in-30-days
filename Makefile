LINKER_SCRIPT=binary.ls

.PHONY: all
all: ipl.img os-init.img
	gzip -cd hello-os_disk.img.gz > hello-os.img
	dd conv=notrunc if=ipl.img of=hello-os.img bs=512 count=1
	sudo mount -t msdos -o loop,fat=12 hello-os.img root/
	sudo cp os-init.img root/os-init.sys
	sudo umount root/

os-init.sys: os-init.img bootpack.img
	cat os-init.img bootpack.img > os-init.sys

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .o .img

.s.o:
	as --32 $< -o $@

.c.o:
	gcc -m32 -o $@ -c $<

.o.img:
	ld $< -T $*.ls -o $@

