# Makefile for 'hangman'
# Author: Namya Malik

PROG = hangman
OBJS = hangman.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: $(PROG)
	./$(PROG) words.txt

valgrind: $(PROG)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) words.txt

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
