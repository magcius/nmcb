
CFLAGS = -g -O0 -Wall -Werror

all: tsndplay

%.tsnd: %.wav
	python mktsnd.py $^ $@

%.tsnd.bin: %.tsnd
	ld -r -o $@ -z noexecstack --format=binary $^
	objcopy --rename-section .data=.rodata,alloc,load,data,contents $@

tsndplay: nmcb.o tsndplay.o tsndplay.tsnd.bin

clean:
	rm -f *.o *.tsnd *.bin tsndplay
