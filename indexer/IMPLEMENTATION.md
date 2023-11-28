# CS50 TSE Indexer
## Implementation Spec

The indexer goes through all the files in a `pageDirectory` and indexes each word into a `hashtable` that holds `counters` for each word based on their `docID`. It creates an inverted-index data structure in the `hashtablee` and saves it to a `indexFilename`.

## Control flow

The Indexer is implemented in `indexer.c` with four functions

### main

Responsible for parsing the arguments. It checks that the proper number of arguments are given and whether the `pageDirectory` exists and is the pathname of the directory created by crawler. 

### parseArgs

Takes the command line arguments and verifies that they are valid. The first argument is the `pageDirectory`, and we call upon `pagedir_init` to see if it is a valid directory. The second argument is the `indexFilename` that we store the index in, and we see if we are able to open it for file writing.

### indexBuild

Builds an in-memory index from webpage files it finds in `pageDirectory`. It loops through the webpages within the `pageDirectory`, or all the document ID numbers. Then, it passes the valid webpages an their docID to the `indexPage` function. 

### indexPage

Scans a webpage document to add its words to the index. It steps through each of the words within the document and, if the word exists in the index hashtable, increments the count of this word for that docID. The key of each hashtable slot is a specific word, and each item is a counters set that stores each count with a key that is the docID.

## Data Structures

### index

We use the 'index' data structure, which represents the in-memory index that stores the word counts and which documents in a directory that they can be found in. 
        
## Other modules

### pagedir

A re-usable module that handles page saving and produciing Crawler webpages

We use the following functions in pagedir: 
`pagedir_init`: constructs a pathname for the .crawler file in the directory
`get_page`: gets a webpage based on its `pageDirectory` and `docID`

### word

A re-usable module that handles word normalization.

We use the `normalize_word` function to perform this action.

### libcs50

We leverage the modules of libcs50, most notably, `hashtable` and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects

## Function prototypes

### indexer

```c
int main(const int argc, char* argv[]);
bool parseArgs(const int argc, char* argv[]);
index_t* indexBuild(char* pageDirectory, FILE* indexFile);
static void indexPage(index_t* index, webpage_t* page, int docID; 
```

### pagedir

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* page_get(const char* pageDirectory, const int docID);
```

### word

```c
char* normalize_word(char* word);
```

## Testing

### Unit Testing

We have an `indextest` that serves as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

### Integration/system testing

We write a script `testing.sh` that invokes the crawler and indexer several times, with a variety of command line arguments. It starts with a sequence of erroneous arguments, then runs the indexer over several crawler directories. 