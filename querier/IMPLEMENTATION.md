# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

We also have the knowledge unit (https://github.com/cs50winter2022/home/blob/main/knowledge/units/querier.md) that includes many topics

Here we focus on the core subset:

-  Control flow
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan 

## Control Flow

The Querier is implemented in `querier.c` with 10 functions:

### main

The `main` function is responsible for checking for the correct number of command-line arguments. Calls `parseArgs`, assigns the parameters, initializes modules, and calls `querier`, then exits zero. 

### parseArgs

Given arguments from command line, verifies if they are valid.

* for `pageDirectory`, checks if it is a crawler directory
* for `indexFilename`, checks if it can be opened for reading
* if any trouble is found, print an error to stderr, return false boolean to main, which will exit non-zero.

### querier

The `querier` function does most of the work for the querier program. It starts by prompting the user for a query input, determines if the query is valid by calling `tokenizeQuery` and `validateQueryStructure`, gets all the page matches for the query by calling `satisfyQuery`, and outputs/ranks them with `outputMatches`.

### tokenizeQuery

The `tokenizeQuery` function validates if all the characters in the query are letters in the alphabet. If so, it adds them to the array of query words.

### validateQueryStructure

The `validateQueryStructure` function validates if the "ands" and "ors" are correctly placed in the query. They cannot occur at the beginning, end, or consecutively in the query.

### satisfyQuery

The `satisfyQuery` function goes through all the words in the query, and processes them based on occurrences of "and" and "or." If there is an "and," it intersects the adjacent words by calling `intersect`. If there is an "or," it unions the adjacent words by calling `union`. 

### unionize

The `unionize` function takes two words and determines the score based on if one or the other can be found in the index for the given webpage. Calls upon helper function `unionHelper`.

### intersect

The `intersect` function takes two words and determines the score based on if both of them can be found in the index for the given webpage. Calls upon helper function `intersectHelper`.

### copyCounters

The `copyCounters` function copies all of the (docID, score) pairs of one counters set into another. Calls upon helper function `copyCountersHelper`.

### outputMatches

The `outputMatches` function considers the final set of webpage matches for the query and ranks each match by their score (from highest to lowest score).

### prompt

The `prompt` function was taken from the CS50 website. It prompts the user for a query in stdin in the terminal. 

## Data Structures

### index

We use the 'index' data structure, which represents the in-memory index that stores the word counts and which documents in a directory that they can be found in. 

### counters

We use the 'counters' data structure, which stores the document IDs and the number of occurrences of a word in the `pageDirectory/docID` page.

## Other Modules

### pagedir

A re-usable module that handles page saving and validating the given pageDirectory.

We use the following functions in pagedir: 
`valid_dir`: checks if the `pageDirectory` is a Crawler directory of webpages
`get_page`: gets a webpage based on its `pageDirectory` and `docID`

### word

A re-usable module that handles word normalization.

We use the `normalize_word` function to perform this action.

### libcs50

We leverage the modules of libcs50, most notably, `hashtable` and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects

## Function Prototypes

### querier

```c
static bool parseArgs(const int argc, char* argv[]);
static void querier(char* pageDirectory, index_t* index);
static bool tokenizeQuery(char* query, char* words[]);
static bool validateQueryStructure(char* allWords[], int numWords);
static counters_t* satisfyQuery(char* allWords[], index_t* index, int numWords);
static void unionize(counters_t* final, counters_t* init);
static void unionHelper(void* arg, const int docID, const int score);
static void intersect(counters_t* init, counters_t* temp);
static void intersectHelper(void* arg, const int docID, const int score);
static void copyCounters(counters_t* init, counters_t* indexCtrs);
static void copyCountersHelper(void* arg, const int key, const int count);
static void outputMatches(char* pageDirectory, counters_t* final);
static void countDocs(void* arg, const int docID, const int score);
static void rankDocs(void* arg, const int docID, const int score);
int fileno(FILE* stream);
static void prompt(void);
```

### pagedir

```c 
bool pagedir_init(const char* pageDirectory);
bool valid_dir(const char *pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* page_get(const char* pageDirectory, const int docID);
```

### word

```c
char* normalize_word(char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `valid_dir` returns false if there is any trouble reading the `.crawler` file. The `webpage` module returns false when URLs are not retrievable, and the Querier does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments. 

First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.

Second, runs over a variety of indexer and crawler outputs in the shared TSE output folder. We use `fuzzyquery.c` to provide a series of random queries to test the querier. 

Third, a run with valgrind over a moderate-sized test case (such as `letters` at depth 3).
