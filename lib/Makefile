LIBS := sprintf.o descriptor.o

.PHONY: all
all: $(LIBS)

.PHONY: test
test: $(LIBS)
	for test in test_*.c; do \
		filename=$${test%.c} \
		lib=$${filename#test_}.o \
		temp=$$(mktemp); \
		gcc $${test} && ./a.out > $$temp; \
		gcc -m32 -march=i486 -std=c99 $${lib} $${test} && ./a.out | diff - $$temp \
		&& echo "$${lib}: TEST PASSED!!" >&2; \
	done

.PHONY: clean
clean:
	rm -f $(LIBS)

.c.o:
	gcc -m32 -march=i486 -std=c99 -fno-stack-protector -o $@ -c $<
