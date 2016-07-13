NAME=tps
CC=clang
CFLAGS=-Wall -O2 -Wpedantic -Wextra -std=gnu11
LDLIBS=

all: new legacy


new:
	$(CC) $(CFLAGS) $(LDLIBS) $(NAME).c -o $(NAME)2

legacy:
	$(CC) $(CFLAGS) $(LDLIBS) -DLEGACY $(NAME).c -o $(NAME)
	
clean:
	rm -f *.o $(NAME) *~
