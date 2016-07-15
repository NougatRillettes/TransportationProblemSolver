NAME=tps
CC=clang
CFLAGS=-Wall -O2 -Wpedantic -Wextra -Wno-gnu -std=gnu11
LDLIBS=

tps: *.o
	$(CC) $(CFLAGS) $(LDLIBS) *.c -o $(NAME)
	
clean:
	rm -f *.o $(NAME) *~
