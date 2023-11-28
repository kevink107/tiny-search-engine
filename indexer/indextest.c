/* 
 * indextest.c - CS50 indextest file
 * 
 * Loads an existing index into an inverted-index data structure and writes that 
 * index to a new file
 * 
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include "hashtable.h"
 #include "index.h"

int 
main(const int argc, char* argv[])
{
  if (argc != 3) {
    fprintf(stderr, "usage: ./indextest [oldIndexFilename] [newIndexFilename]");
    exit(1);
  }

  char* oldIndexFilename = argv[1];
  char* newIndexFilename = argv[2];
  FILE* oldFile;
  FILE* newFile;
  
  // open old index file for reading
  if ((oldFile = fopen(oldIndexFilename, "r")) == NULL) {
    fprintf(stderr, "Could not open old index file: %s", oldIndexFilename);
    exit(2);
  }

  // open new index file for writing
  if ((newFile = fopen(newIndexFilename, "w")) == NULL) {
    fprintf(stderr, "Could not open new index file: %s", newIndexFilename);
    exit(2);
  }

  // save old index into new
  index_t* index = index_get(oldFile);
  index_save(index, newFile);
  index_delete(index);
  
  // close files and exit 0 if successful
  fclose(oldFile);
  fclose(newFile);
  exit(0);
}