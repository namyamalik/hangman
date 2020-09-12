# Makefile for 'hangman'
# Author: Namya Malik

PROG = hangman
OBJS = hangman.o

# uncomment the following to turn on progress-testing print messages
# UNITTESTING=-DTESTPROGRESS

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: $(PROG)
	./$(PROG) words.txt

valgrind: $(PROG)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROG) words.txt

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
