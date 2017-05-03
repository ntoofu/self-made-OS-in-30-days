LINKER_SCRIPT=binary.ls

.PHONY: all
all: ipl.img os-init.sys
	gzip -cd hello-os_disk.img.gz > hello-os.img
	dd conv=notrunc if=ipl.img of=hello-os.img bs=512 count=1
	sudo mount -t vfat hello-os.img root/
	sudo cp os-init.sys root/
	sudo umount root/

ipl.img: ipl.o
	ld ipl.o -T $(LINKER_SCRIPT) -o $@

os-init.sys: os-init.o
	ld os-init.o -T $(LINKER_SCRIPT) -o $@

.s.o:
	as --32 $< -o $*.o
