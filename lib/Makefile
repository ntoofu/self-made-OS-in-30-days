LIBS := sprintf.o

.PHONY: all
all: $(LIBS)

.PHONY: test
test: $(LIBS)
	for lib in $(LIBS); do \
		temp=$$(mktemp); \
		gcc test_$${lib%.o}.c && ./a.out > $$temp; \
		gcc -m32 -march=i486 -std=c99 $${lib} test_$${lib%.o}.c && ./a.out | diff - $$temp \
		&& echo "TEST PASSED!!" >&2; \
	done

.PHONY: clean
clean:
	rm -f $(LIBS)

.c.o:
	gcc -m32 -march=i486 -std=c99 -fno-stack-protector -o $@ -c $<
