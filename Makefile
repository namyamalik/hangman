# Makefile for 'hangman'
# Author: Namya Malik

PROG = hangman
OBJS = hangman.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test clean

test: $(PROG)
	./$(PROG) words.txt

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
