
CFLAGS = -g -O0 -Wall -Werror

all: tsndplay

tsndplay: nmcb.o tsndplay.o tsnddata.o

tsnddata.c: tsnd.wav
	python mktsnd.py $^ $@

clean:
	rm -f *.o tsnddata.c
