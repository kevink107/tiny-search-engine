# Indexer
## CS50 Lab 5
### Kevin King, CS50 Winter 2022

### Indexer

The TSE indexer program goes through all the files in a `pageDirectory` and indexes each word into a `hashtable` that holds `counters` for each word based on their `docID`. It creates an inverted-index data structure in the `hashtable` and saves it to a `indexFilename`. The Indexer is implemented in `indexer.c`.

### Usage
usage: `./indexer [pageDirectory] [indexFilename]`

### Information
Please see the `IMPLEMENTATION.md`, `DESIGN.md`, and `REQUIREMENTS.md` specs for more information.

### Assumptions
No assumptions beyond those provided in the spec. The current directory that is given must be created before the Indexer program is run.

### Return Values
0: Success

1: Usage error

2: Invalid arguments given

### Files
* `Makefile` - compilation procedure
* `indexer.c` - implementation for indexer
* `indextest` - unit test for indexer
* `testing.sh` - shell script test
* `testing.out` - result of `make test &> testing.out`

### Compilation
To commpile, simply `make indexer`

### Testing
To test, simply `make test`. See `testing.out` for details of testing and an example test run. 

To clean up, run `make clean`

To determine whether an index file is the expected/correct output, we use the `indexcmp` program in the shared folder. This takes two arguments, each a pathname to an index file, and prints out any apparent differences. Its usage goes like this: `~/cs50-dev/shared/tse/indexcmp [indexFilenameA] [indexFilenameB]`

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`

To test with valgrind in Terminal - `valgrind --leak-check=full --show-leak-kinds=all ./indexer [pageDirectory] [indexFilename]`

