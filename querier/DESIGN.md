# CS50 TSE Querier
## Design Spec

According to the [Querier Requirements Spec] (REQUIREMENTS.md), the TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
./querier [pageDirectory] [indexFilename]
```

Example...

### Input and outputs

**Input**:

**Output**: 

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*
 2. *parseArgs*
 3. *querier*
 4. *tokenizeQuery*
 5. *validateQueryStructure*
 6. *satisfyQuery*
 7. *unionize*
 8. *intersect*
 9. *copyCounters*
 10. *outputMatches*
 11. *prompt*

### Pseudo code for logic/algorithmic flow

The querier will run as follows: 

  call parseArgs on command line, validate parameters, initialize modules
  call querier

where *parseArgs*: 

  output usage error if
    the pageDirectory is not a crawler directory
    the index cannot be opened for reading

where *querier*:

    calls prompt to get an input query
    reads the input line from stdin
      calls upon tokenizeQuery to determine all characters are letters
      if all characters are letters
        normalize each of the words
        calls validateQueryStructure on the query
        calls satisfyQuery on the index and all query words
        calls outputMatches on the pageDirectory and final counters set of page matches
    calls prompt to get another input query and run querier again

where *tokenizeQuery*: 

    loops through all the characters in the query
        if all the characters that make up a word are letters
          increment the total number of words
        otherwise, the query has invalid characters

where *validateQueryStructure*:

    loops through all the words in the query
      output error message if
        the first word in the query is "and" or "or"
        the last word in the query is "and" or "or"
        there are consecutive "ands" or "ors" in the query

where *satisfyQuery*:

    initializes counters set 'final' for all page matches for the query
    initializes counters set 'init'
    calls copyCounters on 'init' and the index for the first word
    loops through all the words in the query
      if "or" occurs in the query
        calls unionize on 'final' and 'init'
        set 'init' to the counters set for the next word in the query
        moves onto the next word in the query
      otherwise (if the word is not "and" or "or")
        treat a space as "and"
        initializes counters set 'temp'
        calls copyCounters on 'temp' and the counters set for the word
        calls intersect on 'temp' and 'init'
        moves onto the next wor in the query
    calls unionize on 'final' and 'init'
    returns the 'final' counters set

where *unionize*: 

    initializes struct 'both' that stores the two counters sets
    iterates 'init' and stores combined counts for 'final' and 'init' into 'final'

where *intersect*:

    initializes struct 'both' that stores the two counters sets
    iterates 'init' and stores the lower count between 'temp' and 'init' into 'init'

where *copyCounters*:

    stores the counters set for the word found in the index into the other counters set

where *outputMatches*: 

    gets the number of document matches for the query
    creates an array that stores the highest docID and its score
    iterates through the final counters set and finds the highest scored document match
      stores the docID and its score into the array
    get the webpage for the highest ranked document match
    print the querier output format for the document
    set that document's score to 0

where *prompt* (from CS50 website):

    if the file descriptor refers to stdin in terminal
      prints a query prompt message to the user

### Major data structures

We use the following data structures:

- The *index* is *hashtable* keyed by *word* to *(docID, count)* pairs
- The *counters* is keyed by *docID* and stores a count of the number of occurrences of a word in a document with that ID

### Testing plan

*Integration testing*. The *querier*, as a complete program, will be tested by running the *indexer* and *crawler* programs to create a directory of pages and indices. 

1. Test `querier` with various invalid arguments. 
  2. no arguments
  3. one argument
  4. three or more arguments
  5. invalid `pageDirectory`
  6. invalid `indexFilename`
7. Run *querier* on a variety of pageDirectories and indices from the shared TSE output folder 
8. Run *valgrind* on *querier* to ensure no memory leaks or errors. 

