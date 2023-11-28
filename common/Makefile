# Makefile for 'pagedir.c'
#
# Kevin King, February 3, 2022

OBJS = pagedir.o index.o ../libcs50/webpage.o ../libcs50/mem.o ../libcs50/file.o
LIBS = common.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make
# for memory-leak tests
#VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# build library
$(LIBS): $(OBJS)
		ar cr $(LIBS) $(OBJS)

pagedir.o: pagedir.h webpage.h mem.h file.h
../libcs50/webpage.o: webpage.h

index.o: index.h pagedir.h hashtable.h counters.h file.h webpage.h


.PHONY: test valgrind clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f hashtabletest
	rm -f core