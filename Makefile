LIBS := lib/sprintf.o lib/descriptor.o

.PHONY: all
all: ipl.img os-init.sys
	gzip -cd hello-os_disk.img.gz > hello-os.img
	dd conv=notrunc if=ipl.img of=hello-os.img bs=512 count=1
	sudo mount -t msdos -o loop,fat=12 hello-os.img root/
	sudo cp os-init.sys root/os-init.sys
	sudo umount root/

os-init.sys: os-init.img bootpack.img
	cat os-init.img bootpack.img > os-init.sys

bootpack.img: asmfunc.o bootpack.o lib
	ld bootpack.o asmfunc.o $(LIBS) -T bootpack.ls -o $@

.PHONY: lib
lib:
	$(MAKE) -C $@

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .o .img

.s.o:
	as --32 $< -o $@

.c.o:
	gcc -m32 -march=i486 -std=c99 -o $@ -c $<

.o.img:
	ld $< -T $*.ls -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.img
	rm -f os-init.sys
	$(MAKE) clean -C lib

run:
	sudo qemu-system-x86_64 -cpu 486 -drive file=hello-os.img,if=floppy,index=0 -monitor stdio

debug:
	sudo qemu-system-x86_64 -cpu 486 -drive file=hello-os.img,if=floppy,index=0 -S -gdb tcp::1234 &
	sleep 1
	gdb -ex 'target remote localhost:1234'
