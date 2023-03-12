CC=gcc
CFLAGS=-I.
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c = .o)
DEPS = hellomake.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f hellomake

.PHONY: clean