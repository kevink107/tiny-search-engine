/*
 * index.c - CS50 'index' module
 *
 * see index.h for more information
 * 
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "word.h"


/**************** global variables ****************/
typedef hashtable_t index_t;

/**************** local functions ****************/
static void itemprint(void *arg, const char* word, void* counters);
static void printHelper(void *arg, int docID, int count);
static void itemdelete(void *item);

/**************** index_new() ****************/
index_t*
index_new(int num_slots) 
{
  return hashtable_new(num_slots);
}

/**************** index_insert() ****************/
bool
index_insert(index_t* index, char* word, int docID)
{
  // if the word did not already exist in the index
  if (hashtable_find(index, word) == NULL) {
    hashtable_insert(index, word, counters_new());
  }
  // add docID and count to the index 
  if (counters_add(hashtable_find(index, word), docID) > 0) {
    return true;
  }
  return false; 
}

/**************** index_save() ****************/
void 
index_save(index_t* index, FILE* fp)
{
  if (fp != NULL) {
    hashtable_iterate(index, fp, itemprint);
  }
}

/**************** index_delete() ****************/
void 
index_delete(index_t* index)
{
  if (index != NULL) {
    hashtable_delete(index, itemdelete);
  }
}

/**************** index_get() ****************/
index_t*
index_get(FILE* indexFile) 
{
  // CHECK THIS BEFORE SUBMITTING
  index_t* index = (index_t*) index_new(file_numLines(indexFile));

  if (index == NULL || indexFile == NULL) {
    fprintf(stderr, "*** Could not open indexFile");
    return NULL;
  }

  char* word = NULL;
  int docID = 0;
  int count = 0;
  
  // loop through each of the words in the index file
  while ((word = file_readWord(indexFile)) != NULL) {
    counters_t* ctrs = counters_new();

    // add all of the (docID, count) pairs for each word
    while (fscanf(indexFile, "%d %d", &docID, &count) == 2) {
      counters_set(ctrs, docID, count);
      hashtable_insert(index, word, ctrs);
    }
    mem_free(word);
  }
  return index;
}

/**************** itemprint() ****************/
static void
itemprint(void *arg, const char* word, void* counters) 
{
  FILE* fp = arg;
  if (fp != NULL) {
    // only print the words and each of their (docID, count) pairs in ctrs set
    fprintf(fp, "%s ", word);
    counters_iterate(counters, fp, printHelper);
    fprintf(fp, "\n");
  }
}

/**************** counterprint() ****************/
static void
printHelper(void *arg, int docID, int count)
{
  FILE* fp = arg;
  if (fp != NULL) {
    fprintf(fp, "%d %d ", docID, count);
  }
}

/**************** counterdelete() ****************/
static void
itemdelete(void *item)
{
  counters_t* ctrs = item;
  if (ctrs != NULL) {
    counters_delete(ctrs);
  }
}