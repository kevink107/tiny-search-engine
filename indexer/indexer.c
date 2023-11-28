/* 
 * indexer.c - CS50 indexer file
 * 
 * usage: ./indexer [pageDirectory] [indexFilename]
 *    pageDirectory is the pathname of a crawler directory
 *    indexFilename is the pathname of a file where an index is written
 * 
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "webpage.h"
#include "mem.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"

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

/**************** indexBuild ****************/
/* Builds an in-memory index from webpage files it finds in the pageDirectory
 * 
 * We do:
 *    get all the pages in the pageDirectory by document ID
 *    passes the index, webpage, and ID into the indexPage function
 * Parameters:
 *    *pageDirectory - character string for the existing diectory of webpages
 *    *indexFile - the file that will store the index
 */
index_t* indexBuild(char* pageDirectory, FILE* indexFile);

/**************** indexPage ****************/
/* Scans a webpage document to add its words to the index
 * 
 * We return: 
 *    true, if each word was added 
 * Parameters:
 *    *index - the index of words from the .crawler file
 *    *page - the webpage to be scanned
 *    docID - the document ID of the webpage
 */
static void indexPage(index_t* index, webpage_t* page, int docID); 


/**************** functions ****************/

/**************** main ****************/
int 
main(const int argc, char* argv[]) 
{
  if (argc != 3) {
    fprintf(stderr, "usage: ./indexer [pageDirectory] [indexFilename]\n");
    exit(1);
  }

  // initialize the parameters
  char* pageDirectory = NULL;
  char* indexFilename = NULL;
  FILE* indexFile;

  // parse the comand line arguments and check their validity
  if (parseArgs(argc, argv) == true) {
    pageDirectory = argv[1];
    indexFilename = argv[2];
  }
  else {
    exit(2);
  }
  
  // open the indexFile for writing and build the index
  indexFile = fopen(indexFilename, "w");
  indexBuild(pageDirectory, indexFile);
  
  // close the indexFile and set the parameters back to NULL
  fclose(indexFile);
  pageDirectory = NULL;
  indexFilename = NULL;
  exit(0);
}

/**************** parseArgs ****************/
static bool
parseArgs(const int argc, char* argv[])
{
  FILE *fp;
  
  // check if valid crawler directory
  if (valid_dir(argv[1]) == false) {
    fprintf(stderr, "*** Not a valid directory: %s\n", argv[1]);
    return false;
  }
  // check if the index can be opened for writing
  else if ( (fp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "*** Could not open index for writing: %s\n", argv[2]);
    return false;
  }
  else {
    fclose(fp);
    return true;
  }
}

/**************** indexBuild ****************/
index_t*
indexBuild(char* pageDirectory, FILE* indexFile) 
{
  // create empty index and new webpage struct
  index_t* index = (index_t*) index_new(100);
  webpage_t* currPage; 
  int id = 1;

  // loop through all the documents in the directory given
  while ((currPage = page_get(pageDirectory, id)) != NULL) {
    // load each webpage and pass it to indexPage along with docID
    indexPage(index, currPage, id); 
    id++;
    webpage_delete(currPage);
  }
  // save the index to a file, then delete the index (free memory space)
  index_save(index, indexFile);
  index_delete(index);
  return index;
}

/**************** indexPage ****************/
static void
indexPage(index_t* index, webpage_t* page, int docID)
{
  int pos = 0;
  char* word;

  // loop through each word in the webpage  
  while((word = webpage_getNextWord(page, &pos))){
    // only take words that are at least 3 characters long
    if (strlen(word) >= 3) {
      // normalize each word and insert it into index with docID
      word = normalize_word(word);
		  index_insert(index, word, docID);
    }
		mem_free(word);
  }
} 
 