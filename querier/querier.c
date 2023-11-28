/* 
 * querier.c - CS50 querier file
 * 
 * usage: ./querier [pageDirectory] [indexFilename]
 *    pageDirectory is the pathname of a crawler directory
 *    indexFilename is the pathname of an index file to be read
 * 
 * Kevin King
 * CS50 Lab 6
 * Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"


/**************** local types ****************/
typedef struct ctrspair {
  counters_t* first;
  counters_t* second;
} ctrspair_t;


/* function prototype *declarations*; the functions are *defined* below */

/**************** parseArgs ****************/
/* Takes command line arguments and extracts them into the proper parameters
 * 
 * We return: 
 *    true, if arguments are verified
 *    false, if arguments don't meet requirements
 * Parameters:
 *    argc - integer count of command line arguments
 *    *argv - pointer to the array of command line arguments
 */
static bool parseArgs(const int argc, char* argv[]);

/**************** querier ****************/
/* Prompts the user for an input query, determines if the query is valid, 
 * processes the query, and outputs the ranked webpages
 * 
 * We do: 
 *    gets all input words and processes them for the query
 * Parameters:
 *    *pageDirectory - character string for the existing diectory of webpages
 *    *index - index of words from the .crawler file
 */
static void querier(char* pageDirectory, index_t* index);

/**************** tokenizeQuery ****************/
/* Processes the query and tokenizes its words 
 * 
 * We return:
 *    true - if all characters are letters
 *    false - i invalid characters includes
 * Parameters:
 *    *query - the character string for the input query
 *    *words[] - pointer to the array of all input words
 */
static bool tokenizeQuery(char* query, char* words[]);

/**************** validateQueryStructure ****************/
/* Determines if the query is of valid structure 
 * 
 * We do: 
 *    ensures the 'ands' and 'ors' are properly placed
 * Parameters:
 *    *words[] - pointer to the array of all the input words
 *    numWords - total number of words in the input
 */
static bool validateQueryStructure(char* words[], int numWords);

/**************** satisfyQuery ****************/
/* Create set of documents that satisfy the input query
 * 
 * We return:
 *    counters set of all document matches for the query
 * Parameters:
 *    *words[] - pointer to the array of all the input words
 *    *index - index of words from the .crawler file
 *    numWords - total number of words in the input
 */
static counters_t* satisfyQuery(char** words, index_t* index, int numWords);

/**************** unionize ****************/
/* Processes the 'ors' in the query and unions the query accordingly
 * 
 * We do:
 *    updates query and calls upon unionHelper
 * Parameters:
 *    *final - counters set that will be updated 
 *    *init - existing counters set for a query word - to be added to final
 */
static void unionize(counters_t* final, counters_t* init);

/**************** unionHelper ****************/
/* Helper function for unionize
 * 
 * We do:
 *    updates score by union for a query 
 * Parameters:
 *    *arg - arbitrary void argument - init
 *    docID - key for the 'final' counters set in unionize
 *    score - count for the 'final' counters set in unionize
 */
static void unionHelper(void* arg, const int docID, const int score);

/**************** intersect ****************/
/* Processes the 'ands' in the query and intersects the query accordingly
 * 
 * We do:
 *    updates query and calls upon intersectHelper
 * Parameters:
 *    *init - counters set that will be updated 
 *    *temp - existing counters set for a query word - to be added to init
 */
static void intersect(counters_t* init, counters_t* temp);

/**************** intersectHelper ****************/
/* Helper function for intersect
 * 
 * We do:
 *    updates score by inteersection for a query 
 * Parameters:
 *    *arg - arbitrary void argument - temp
 *    docID - key for the 'init' counters set in intersect
 *    score - count for the 'init' counters set in intersect
 */
static void intersectHelper(void* arg, const int docID, const int score);

/**************** copyCounters ****************/
/* Copies a counters set from one that correlates to the index for a word of interest
 *
 * We do:
 *    copies one counters set to another by calling copyCountersHelper
 * Parameters:
 *    *init - counters set that will store indexCtrs
 *    *indexCtrs - existing counters set for a specific word
 */
static void copyCounters(counters_t* init, counters_t* indexCtrs);

/**************** copyCountersHelper ****************/
/* Helper function for copyCounters
 * 
 * We do:
 *    sets each counters set key and count from one index to another
 * Parameters:
 *    *arg - arbitrary void argument - indexCtrs
 *    key - key for the 'indexCtrs' counters set in copyCounters
 *    count - count for the 'indexCtrs' counters set in copyCounters
 */
static void copyCountersHelper(void* arg, const int key, const int count); 


/**************** outputMatches ****************/
/* Outputs all the final document matches for the query
 * 
 * We do:
 *    calls upon rankDocs and countDocs to organize the output
 * Parameters:
 *    *pageDirectory - character string for the existing diectory of webpages
 *    *final - the final counters set for the query's document matches
 */
static void outputMatches(char* pageDirectory, counters_t* final);

/**************** countDocs ****************/
/* Helper function for outputMatches
 * 
 * We do:
 *    counts the number of documents in the final counters set
 * Parameters:
 *    *arg - arbitrary void argument
 *    docID - key for the 'indexCtrs' counters set in copyCounters
 *    count - count for the 'indexCtrs' counters set in copyCounters
 */
static void countDocs(void* arg, const int docID, const int score);

/**************** rankDocs ****************/
/* Helper function for outputMatches
 * 
 * We do:
 *    sorts each of the documents by score
 * Parameters:
 *    *arg - arbitrary void argument
 *    docID - key for the 'indexCtrs' counters set in copyCounters
 *    count - count for the 'indexCtrs' counters set in copyCounters
 */
static void rankDocs(void* arg, const int docID, const int score);

/**************** fileno ****************/
/* The fileno() function is provided by stdio,
 * but for some reason not declared by stdio.h, so declare here.
 * Taken from CS50 website
 */
int fileno(FILE* stream);

/**************** prompt ****************/
/* Prompts user for a query
 * Taken from CS50 website 
 */
static void prompt(void);


/**************** functions ****************/

/**************** main ****************/
int 
main(const int argc, char* argv[])
{
  if (argc != 3) {
    fprintf(stderr, "usage: ./querier [pageDirectory] [indexFilename]\n");
    exit(1);
  }

  // initialize the parameters
  char* pageDirectory = NULL;
  char* indexFilename = NULL;
  
  // parse the comand line arguments and check their validity
  if (parseArgs(argc, argv) == true) {
    pageDirectory = argv[1];
    indexFilename = argv[2];
  }
  else {
    exit(2);
  }

  // open the provided index file
  FILE* indexFile = fopen(indexFilename, "r");

  index_t* index = index_get(indexFile);
  if (index == NULL) {
    fprintf(stderr, "*** Could not get index");
    exit(3);
  }

  // call querier on the index
  querier(pageDirectory, index);
  index_delete(index);  
  pageDirectory = NULL;
  indexFilename = NULL;
  fclose(indexFile);
  exit(0); 
  
}

/**************** parseArgs ****************/
static bool 
parseArgs(const int argc, char* argv[])
{
  FILE *fp;
  // check if valid crawler directory
  if (valid_dir(argv[1]) == false) {
    return false;
  }
  // check if the index can be opened for reading
  else if ((fp = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "*** Could not open index for reading: %s\n", argv[2]);
    return false;
  }
  else {
    fclose(fp);
    return true;
  }
}

/**************** querier ****************/
static void 
querier(char* pageDirectory, index_t* index) 
{ 
  // prompt user for an input query
  prompt();
  char* query;

  // read the line of stdin 
  while ((query = file_readLine(stdin)) != NULL ) {
    int numWords = 0;
    // loop through each character in the query
    for (int i = 0; i < strlen(query); i++) {
      // check the character is a letter
      if (isalpha(query[i])) {
        numWords++;
        while (isalpha(query[i+1])) {
          i++;
        }
      }
    }
    char* words[numWords];
    bool valid = tokenizeQuery(query, words);

    // if the query is made up of all letters and spaces
    if (valid) {
       // print words in the query
      printf("Query: ");
      for (int i = 0; i < numWords; i++) {
        printf("%s ", words[i]);
      }
      printf("\n");
      
      if (validateQueryStructure(words, numWords)) {
        counters_t* final = satisfyQuery(words, index, numWords);
        outputMatches(pageDirectory, final);
        counters_delete(final);
      }
      printf("----------------------------------------------------------------------------\n");
    }
    else {
      fprintf(stderr, "*** Invalid query structure\n");
    }
    
    // free the query and prompt for another input query
    mem_free(query);
    prompt(); 
  }
  printf("\n");
}

static bool
tokenizeQuery(char* query, char* words[]) 
{
  int curr = 0;
  char* word = query;
  char* rest = query;
  while (*word != '\0') {
    // if neither a letter nor space
    if (!isspace(*word) && !isalpha(*word)) {
      return false;
    }
    if (isalpha(*word)) {
      // add character to current index
      words[curr] = word;
      curr++;
      rest = word;
      // loop through the rest of the word
      while(isalpha(*rest)) {
        rest++;
      }
      word = rest;
      // adds terminator at the end of each word  
      if (*rest != '\0') {
        *rest = '\0';
        word++;
      }
      // normalize each word in the array
      normalize_word(words[curr-1]);
    }
  }
  return true;
}

/**************** validateQueryStructure ****************/
static bool 
validateQueryStructure(char* words[], int numWords)
{
  if (numWords < 1) {
    fprintf(stderr, "*** Invalid query\n");
    return false;
  }
  for (int i = 0; i < numWords; i++) {
    // finds if a word is and/or
    int andCmp = strcmp(words[i], "and");
    int orCmp = strcmp(words[i], "or");

    // cannot have and/or at beginning of sentence
    if ( (andCmp == 0) || (orCmp == 0) ) {
      if (i == 0) {
        fprintf(stderr, "*** Cannot have '%s' be first\n", words[i]);
        return false;
      }
      // cannot have and/or at end of sentence
      else if (i == numWords - 1) {
        fprintf(stderr, "*** Cannot have '%s' be last\n", words[i]);
        return false;
      }
      // check if there are consecutive and/or 
      else {
        andCmp = strcmp(words[i+1], "and");
        orCmp = strcmp(words[i+1], "or");
        if ( (andCmp == 0) || (orCmp == 0) ) {
          fprintf(stderr, "*** Cannot have consecutive 'and' or 'or' in query\n");
          return false;
        }
      }
    }
  }
  return true;
}

/**************** satisfyQuery ****************/
static counters_t* 
satisfyQuery(char* words[], index_t* index, int numWords)
{
  counters_t* final = counters_new(); // the final counter set that we will return 
  counters_t* init = counters_new();  // will load the counter set for a certain word
  copyCounters(init, hashtable_find(index, words[0]));
  if (init == NULL) {
    return NULL;
  }
  // loop through each of the words
  for (int i = 0; i < numWords; i++) {
    // determine if a word is and/or
    int andCmp = strcmp(words[i], "and");
    int orCmp = strcmp(words[i], "or");

    // checks for ors
    if (orCmp == 0) {
      unionize(final, init); // union the adjacent words
      init = counters_new();
      copyCounters(init, hashtable_find(index, words[i+1]));
      continue;
    }
    else {
      // if no ors or and, intersects
      if (andCmp != 0) {
        counters_t* temp = counters_new();
        copyCounters(temp, hashtable_find(index, words[i]));
        intersect(init, temp); // intersect the adjacent words
      }
      continue;
    }
  }
  // union the rest - may not do anything here
  unionize(final, init);
  return final;
}

/**************** UNIONIZE ****************/
static void
unionize(counters_t* final, counters_t* init)
{
  // struct to hold both counters
  ctrspair_t* both = (ctrspair_t*) mem_malloc(sizeof(ctrspair_t));
  both->first = final;
  both->second = init;
  counters_iterate(init, both, unionHelper);
  counters_delete(init);
  mem_free(both);
}

/**************** unionHelper ****************/
static void
unionHelper(void* arg, const int docID, const int score)
{
  ctrspair_t* both = arg;
  int wordCount = counters_get(both->first, docID);
  counters_set(both->first, docID, score + wordCount); // sets final set score to combined score
}

/**************** INTERSECT ****************/
static void
intersect(counters_t* init, counters_t* temp)
{
  // struct to hold both counters
  ctrspair_t* both = (ctrspair_t*) mem_malloc(sizeof(ctrspair_t));
  both->first = init;
  both->second = temp;
  counters_iterate(init, both, intersectHelper);
  counters_delete(temp);
  mem_free(both);
}

/**************** intersectHelper ****************/
static void
intersectHelper(void* arg, const int docID, const int score)
{
  ctrspair_t* both = arg;
  int wordCount = counters_get(both->second, docID); // word count for 'init'
  if (wordCount == 0) {
    counters_set(both->first, docID, 0);
  }
  // sets to the lower of the scores 
  if (wordCount < score) {
    counters_set(both->first, docID, wordCount);
  }
  else {
    counters_set(both->first, docID, score);
  }
  
}

/**************** COPYCOUNTERS ****************/
static void 
copyCounters(counters_t* init, counters_t* indexCtrs)
{
  counters_iterate(indexCtrs, init, copyCountersHelper);
}

/**************** copyCountersHelper ****************/
static void 
copyCountersHelper(void* arg, const int key, const int count) 
{
  counters_set(arg, key, count);
}

/**************** outputQuery ****************/
static void
outputMatches(char* pageDirectory, counters_t* final)
{
  if (final == NULL) {
    printf("No documents match\n");
  }
  else {
    int numDocs = 0;
    counters_iterate(final, &numDocs, countDocs);
    // output of the query matches based on number of matches found
    if (numDocs == 0) {
      printf("Matches 0 documents: \n");
    }
    else if (numDocs == 1) {
      printf("Matches 1 document: \n");
    }
    else {
      printf("Matches %d documents (ranked):\n", numDocs);
    }

    // loop through all the documents and get the highest ranked one
    int checkedDocs = 0;
    while (checkedDocs < numDocs) {
      // find doc with highest scored docID
      int highest[2];
      highest[0] = 0; // represents the docID
      highest[1] = 0; // represents the score 

      // sort each of the documents by score then docID
      int currDoc = 0;
      while (currDoc < numDocs) {
        counters_iterate(final, highest, rankDocs);
        currDoc++;
      }

      webpage_t* currPage = page_get(pageDirectory, highest[0]);
      printf("  score %2d  doc %2d: %s\n", highest[1], highest[0], webpage_getURL(currPage));
      counters_set(final, highest[0], 0);
      webpage_delete(currPage);

      checkedDocs++;
    }
  }
}

/**************** countDocs ****************/
static void countDocs(void* arg, const int docID, const int score) {
  int* nitems = arg;
  if (nitems != 0 && docID >= 1 && score != 0) {
    (*nitems)++;
  }
}

/**************** rankDocs ****************/
static void rankDocs(void* arg, const int docID, const int score) {
  int* highest = arg;
  if (score > highest[1]) {
    highest[0] = docID;
    highest[1] = score;
  }

}

/**************** prompt ****************/
static void 
prompt(void)
{
  // print a prompt if stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}