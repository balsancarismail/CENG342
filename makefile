CC=gcc
CFLAGS=-I.
DEPS = hellomake.h
OBJ = 18050111021.o hellofunc.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f hellomake

.PHONY: clean
