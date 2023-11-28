# Makefile for 'crawler'
#
# Kevin King, January 27, 2022

OBJS = crawler.o ../libcs50/bag.o ../libcs50/file.o ../libcs50/hash.o ../libcs50/hashtable.o
OBJS += ../libcs50/mem.o ../libcs50/set.o ../libcs50/webpage.o ../common/pagedir.o
LIBS = ../libcs50/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
CC = gcc
MAKE = make

crawler: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# dependencies
crawler.o: ../libcs50/bag.h ../libcs50/file.h ../libcs50/hash.h ../libcs50/hashtable.h ../libcs50/mem.h ../libcs50/set.h ../libcs50/webpage.h ../common/pagedir.h
../libcs50/webpage.o: ../libcs50/webpage.h

.PHONY: test valgrind clean

test: crawler testing.sh
		bash -v ./testing.sh 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core