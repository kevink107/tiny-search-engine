# Makefile for 'querier'
#
# Kevin King, CS50 Lab 6, Winter 2022

OBJS = ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/hash.o ../libcs50/counters.o 
OBJS += ../libcs50/mem.o ../libcs50/file.o ../common/index.o ../common/word.o ../common/pagedir.o
LIBS = ../libcs50/libcs50.a ../common/common.a

#FLAGS = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make


all: querier fuzzquery

querier: $(OBJS) querier.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: $(OBJS) fuzzquery.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: ../common/index.h ../common/pagedir.h ../common/word.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/mem.h ../libcs50/webpage.h

fuzzquery.o: ../libcs50/mem.h ../libcs50/file.h

.PHONY: clean

test: querier testing.sh
		bash -v ./testing.sh

clean: 
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f core