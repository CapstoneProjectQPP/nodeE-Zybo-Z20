CC = gcc
CFLAGS = -g -Wmissing-prototypes -Werror -Wextra

all: main

main.o: main.c
	$(CC) $(CFLAGS) -c $^

main: main.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o main

erase:
	rm -f *.c *.o *.h Makefile main