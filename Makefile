NAME=tps
CC=clang
CFLAGS=-Wall -O2 -Wpedantic -Wextra -std=gnu11
LDLIBS=

main: $(NAME)

clean:
	rm -f *.o $(NAME) *~
