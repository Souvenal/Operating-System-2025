TLIB_PATH := ../../thread-lib

CFLAGS := -O2 -I$(TLIB_PATH) -Wno-unused-result

all: orchestra orchestra-cv

notes/C1.wav:
	python3 gen-notes.py

orchestra: orchestra.c main.c notes/C1.wav
	gcc $(CFLAGS) -o $@ $<

orchestra-cv: orchestra-cv.c main.c notes/C1.wav
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf notes/ orchestra orchestra-cv

