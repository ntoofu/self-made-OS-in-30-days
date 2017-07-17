LIBS := util/sprintf.o os/descriptor.o os/asmfunc.o graphic/display.o

.PHONY: all
all: $(LIBS)

.PHONY: test
test: $(LIBS)
	for test in $$(find -type f -name 'test_*.c'); do \
		libsrc=$${test/test_/} \
		libobj=$${libsrc%.c}.o \
		temp=$$(mktemp); \
		gcc $${test} && ./a.out > $$temp; \
		gcc -m32 -march=i486 -std=c99 $${libobj} $${test} && ./a.out | diff - $$temp \
		&& echo "$${libobj}: TEST PASSED!!" >&2; \
		rm -f $${temp}; \
	done

.PHONY: clean
clean:
	rm -f $(LIBS)

.c.o:
	gcc -m32 -march=i486 -std=c99 -fno-stack-protector -o $@ -c $<

.s.o:
	as --32 $< -o $@

