TLIB_PATH := ../../thread-lib

CFLAGS := -O2 -I$(TLIB_PATH)

all: sum

sum: sum.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f sum
