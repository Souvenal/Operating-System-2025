CFLAGS := -Wall -g

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=)

# Default target
all: $(OBJS)

%: %.c
	gcc $(CFLAGS) $< -o $@

run: all
	./create-tree > view.md

clean:
	rm -f $(OBJS) view.md

.PHONY: all clean
