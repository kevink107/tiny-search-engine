# Crawler 
## CS50 Lab 4
### Kevin King, CS50 Winter 2022

### Crawler 

The TSE crawler program crawls the web and retrieves webpages that start with a given "seed" URL. It parses through the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth." The Crawler is defined in `crawler.h` and implemented in `crawler.c`.

### Usage
usage: `./crawler [seedURL] [pageDirectory] [maxDepth]`

### Information
Please see the `IMPLEMENTATION.md`, `DESIGN.md`, and `REQUIREMENTS.md` specs for more information.

### Assumptions
Each depth given is an string type integer. 
The `pageDirectory` does not contain any files whose name is an integer. 

### Return Values
0: Success
1: Usage error
2: Invalid arguments given
3: Failed to create bag or hashtable

### Files
* `Makefile` - compilation procedure
* `crawler.c` - implementation for crawler
* `crawler.h` - interface for crawler
* `testing.sh` - shell script test
* `testing.out` - result of `make test &> testing.out`

### Compilation
To compile, simply `make crawler`

### Testing
To test, simply `make test`. See `testing.out` for details of testing and an example test run. 

To clean up, run `make clean`

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`

To test with valgrind in Terminal - `valgrind --leak-check=full --show-leak-kinds=all ./crawler [seedURL] [pageDirectory] [maxDepth]`