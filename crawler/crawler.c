/* 
 * crawler.c - CS50 crawler file
 * 
 * usage: ./crawler [seedURL] [pageDirectory] [maxDepth]
 *    seedURL is the website link to be crawled - the 'internal' directory
 *    pageDirectory is the (existing) directory where we write downloaded webpages
 *    maxDepth is the integer range that indicates maximum crawl depth
 * 
 * Kevin King, CS50 Winter 2022, February 1, 2022
 */

#define APPTEST
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bag.h"
#include "file.h"
#include "hash.h"
#include "hashtable.h"
#include "mem.h"
#include "set.h"
#include "webpage.h"
#include "pagedir.h"

/* function prototype *declarations*; the functions are *defined* below */

/**************** parseArgs ****************/
/*
 * Takes command line arguments and extracts them into the proper function parameters
 * 
 * We do: 
 *    assign values to function parameters
 *    exits nonzero if arguments don't meet requirements
 * Parameters:
 *    argc - integer count of command line arguments
 *    *argv - pointer to the array of command line arguments
 *    *seedURL - the internal directory, used as initial URL
 *    **pageDirectory - pointer to the existing directory to write downloaded webpages
 *    *maxDepth - pointer to the integer that indicates maximum crawl depth 
 */
static void parseArgs(const int argc, char* argv[], char* seedURL, char** pageDirectory, int* maxDepth);

/**************** crawl ****************/
/*
 * Starts at the seedURL and crawls until a specific maxDepth. Creates
 * the webpages in the pageDirectory given for each internal webpage
 * 
 * We do: 
 *    pageDirectory gets populated with webpages
 * Parameters: 
 *    *seedURL - the internal directory, used as initial URL
 *    *pageDirectory - the existing directory to write downloaded webpages
 *    maxDepth - the integer that indicates maximum crawl depth 
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

/**************** pageScan ****************/
/*
 * Implements pagescanner function. Scans a given webpage to extract any 
 * links (URLs), ignoring non-internal URLs. URLs that haven't been 
 * seen before are added to the bag and hashtable. 
 * 
 * We print:
 *    statements on whether a webpage is fetched or being scanned
 * Parameters: 
 *    *seedURL - the internal directory, used as initial URL
 *    *pageDirectory - the existing directory to write downloaded webpages
 *    maxDepth - the integer that indicates maximum crawl depth 
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** pagefetcher ****************/
/*
 * Fetches the HTMl of a given webpage
 * 
 * We return: 
 *    true on success
 *    false on failure
 * Parameters:
 *    *page - given webpage
 */
static bool pagefetcher(webpage_t* page);

/**************** logr ****************/
/*
 * Log one word (1-9 chars) about a given url      
 * taken from CS50 website 
 * 
 * Parameters: 
 *    *word - string that displays what is being down to the URL
 *    depth - current depth of the URL relative to the seedURL
 *    *url - current URL of matter
 */    
static void logr(const char *word, const int depth, const char *url);

/**************** functions ****************/

/**************** main ****************/
int 
main(const int argc, char* argv[]) 
{
  // command line must have exactly 3 arguments given
  if (argc != 4) {
    fprintf(stderr, "usage: ./crawler [seedURL] [pageDirectory] [maxDepth]\n");
    exit(1);
  }

  char* seedURL = NULL;        // internal directory, used as initial URL
  char* pageDirectory = NULL;  // existing directory in which to write downloaded webpages
  int maxDepth;                // integer in range [0..10], indicates maximum crawl depth

  seedURL = mem_malloc(strlen(argv[1])+1);
  parseArgs(argc, argv, seedURL, &pageDirectory, &maxDepth);

  // allocate memory for a copy of the seedURL
  char *copiedURL = mem_malloc(strlen(seedURL)+1);
  strcpy(copiedURL, seedURL);
  crawl(copiedURL, pageDirectory, maxDepth);

  // free strings before exiting
  mem_free(seedURL);
  mem_free(copiedURL);
  seedURL = NULL;
  pageDirectory = NULL;

  exit(0);
}

/**************** parseArgs ****************/
static void 
parseArgs(const int argc, char* argv[], char* seedURL, char** pageDirectory, int* maxDepth) 
{
  char* url;
  // check if URL can be normalized and is internal
  if ( (url = normalizeURL(argv[1])) != NULL ) {
    if (isInternalURL(url) == false) {
      fprintf(stderr, "*** URL is not internal\n");
      mem_free(url);
      exit(2);
    }
  }
  else {
    fprintf(stderr, "*** Could not normalize URL\n");
    mem_free(url);
    exit(2);
  }
  
  // check that page directory path exists - call pagedir_init()
  if (pagedir_init(argv[2]) == true) {
    *pageDirectory = argv[2];
  }
  else {
    fprintf(stderr, "*** Invalid directory given\n");
    mem_free(url);
    exit(2);
  }

  // check that depth int is in range [0..10]
  for (int i = 0; i < strlen(argv[3]); i++) {
    if (isdigit(argv[3][i])) {
      continue;
    }
    else {
      fprintf(stderr, "*** Invalid depth given\n");
      mem_free(url);
      exit(2);
    }
  }
  if (atoi(argv[3]) >= 0 && atoi(argv[3]) <= 10) {
    *maxDepth = atoi(argv[3]);
  }

  
  strcpy(seedURL,url);
  // free url as it was only used for parseArgs function
  mem_free(url);
}

/**************** crawl ****************/
static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  // initialize bag 
  bag_t* pagesToCrawl;
  pagesToCrawl = bag_new();
  if (pagesToCrawl == NULL) {
    fprintf(stderr,"bag_new function failed\n");
    exit(3);
  }

  // initialize hashtable
  hashtable_t* pagesSeen; 
  pagesSeen = hashtable_new(200);
  if (pagesSeen == NULL) {
    fprintf(stderr,"hashtable_new function failed\n");
    exit(3);
  }

  int id = 1;
  int currentDepth = 0;
  char* url;
  url = normalizeURL(seedURL);

  webpage_t* currPage;
  hashtable_insert(pagesSeen, url, "");
  bag_insert(pagesToCrawl, webpage_new(url, currentDepth, NULL));

  // while bag is not empty
  while ((currPage = (webpage_t*) bag_extract(pagesToCrawl)) != NULL) {
    // pulls webpage from the bag
    if (pagefetcher(currPage)) {   
      logr("Fetched", webpage_getDepth(currPage), webpage_getURL(currPage) );
      
      // saves page to the directory with a unique ID
      pagedir_save(currPage, pageDirectory, id);
      id = id + 1;
      
      currentDepth = webpage_getDepth(currPage);
      if (currentDepth < maxDepth) {   
        logr("Scanning", currentDepth, webpage_getURL(currPage));
        pageScan(currPage, pagesToCrawl, pagesSeen);
      }
    }
    else {
      printf("Could not fetch\n");
    }
    webpage_delete(currPage);
  }
  bag_delete(pagesToCrawl, webpage_delete);
  hashtable_delete(pagesSeen, NULL);
}

/**************** pageScan ****************/
static void 
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  int depth = webpage_getDepth(page);
  int pos = 0;
  char* url;

  // parse through each of the embedded URLs within the webpage
  while ((url = webpage_getNextURL(page, &pos))) {
    logr("Found", depth, url);
    // checks if url is internal and tries to insert into hashtable
    if (isInternalURL(url) == true) {
      if (hashtable_insert(pagesSeen, url, "") == true) {
        // webpage added - to be crawled 
        bag_insert(pagesToCrawl, webpage_new(url, depth+1, NULL));
        logr("Added", depth, url);
      }
      else {
        logr("IgnDupl", depth, url);
        mem_free(url);
      }
    }
    else {
      logr("IgnExtrn", depth, url);
      mem_free(url);
    }
  }
}

/**************** pagefetcher ****************/
static bool 
pagefetcher(webpage_t* page){
  return webpage_fetch(page);
}

/**************** logr ****************/      
static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}



