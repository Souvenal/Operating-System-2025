CFLAGS := -O2 -I$(TLIB_PATH)

all: orchestra orchestra-cv

C1.wav:
	python3 gen-notes.py

orchestra: orchestra.c main.c notes/C1.wav
	gcc $(CFLAGS) -o $@ $<

orchestra-cv: orchestra-cv.c main.c notes/C1.wav
	gcc $(CFLAGS) -o $@ $<



clean:
	rm -f *.wav orchestra orchestra-cv
