# Makefile for 'indexer'
#
# Kevin King, CS50 Lab 5, Winter 2022

OBJS = ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/hash.o ../libcs50/counters.o ../libcs50/set.o
OBJS += ../libcs50/mem.o ../libcs50/file.o ../common/index.o ../common/word.o ../common/pagedir.o 
LIBS = ../libcs50/libcs50.a ../common/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

all: indextest indexer

indextest: $(OBJS) indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer: $(OBJS) indexer.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: ../common/index.o ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/hash.h ../libcs50/set.h ../libcs50/mem.h ../common/pagedir.h
indextest.o: ../common/index.o ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/hash.h ../libcs50/set.h ../libcs50/mem.h ../common/pagedir.h

.PHONY: clean

test: indextest indexer testing.sh
		bash -v ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indextest indexer
	rm -f core