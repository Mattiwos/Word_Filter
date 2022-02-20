CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

SOURCE = $(wildcard *.c)
OBJECT = $(wildcard *.o)

all: banhammer

banhammer: banhammer.o speck.o ht.o ll.o node.o bf.o bv.o parser.o
	$(CC) -o banhammer banhammer.o speck.o ht.o ll.o node.o bf.o bv.o parser.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECT) banhammer

scan-build: clean
	scan-build make
format:
	clang-format -i -style=file *.c *.h
