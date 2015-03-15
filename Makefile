
CFLAGS = -g -O0 -Wall -Werror

all: tsndplay tsndplay2

%.tsnd: %.wav
	python mktsnd.py $^ $@

%.tsnd.bin: %.tsnd
	ld -r -o $@ -z noexecstack --format=binary $^
	objcopy --rename-section .data=.rodata,alloc,load,data,contents $@

tsndplay: nmcb.o tsndplay.o tsndplay.tsnd.bin
tsndplay2: nmcb.o tsndplay2.o

clean:
	rm -f *.o *.tsnd *.bin tsndplay
