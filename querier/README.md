# Querier
## CS50 Lab 6
### Kevin King, CS50 Winter 2022

### Querier

The TSE Querier takes a query from standard input and reads through an index produced by the Indexer and the page files produced by the Crawler, and interactively answers the query. It finds the page matches for the query in a `counters` set and outputs them in ranked order, from highest to lowest score. 

### Usage
usage: `./querier [pageDirectory] [indexFilename]`

### Information
Please see the `IMPLEMENTATION.md`, `DESIGN.md`, and `REQUIREMENTS.md` specs for more information.

### Assumptions
The current directory and indexFilename that are given must be created before the Querier program is run. To end the querier program, type 'ctrl + D' as the query.

### Return Values
0: Success

1: Usage error

2: Invalid arguments given

3: Trouble opening the index

### Files
* `Makefile` - compilation procedure
* `querier.c` - implementation for the querier
* `fuzzyquery.c` - black-box testing procedure
* `testing.sh` - shell script test
* `testing.out` - result of `make test &> testing.out`

### Compilation
To compile, simply `make querier`

### Testing
To test, simply `make test`. See `testing.out` for details of testing and an example test run. 

To clean up, run `make clean`

To test with valgrind in Terminal - `valgrind --leak-check=full --show-leak-kinds=all ./querier [pageDirectory] [indexFilename]`