NAME=tps
CC=clang
CFLAGS=-Wall -O2 -Wpedantic -Wextra -Wno-gnu -std=gnu11
LDLIBS=

all: new legacy


new:
	$(CC) $(CFLAGS) $(LDLIBS) *.c -o $(NAME)2

legacy:
	$(CC) $(CFLAGS) $(LDLIBS) -DLEGACY *.c -o $(NAME)
	
clean:
	rm -f *.o $(NAME) *~
