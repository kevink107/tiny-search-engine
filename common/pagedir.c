/* 
 * pagedir.c - CS50 'pagedir' module
 * 
 * see pagedir.h for more information
 * 
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "mem.h"
#include "webpage.h"
#include "file.h"

/**************** pagedir_init ****************/
bool pagedir_init(const char* pageDirectory) 
{
  char filename[strlen(pageDirectory) + 1];
  strcpy(filename, pageDirectory);
  strcat(filename, "/.crawler");

  FILE* fp;
  if ((fp = fopen(filename,"w")) == NULL) {
    fprintf(stderr, "*** Could not open file: %s\n", filename);
    return false;
  }
  else {
    fclose(fp);
    return true;
  }
}

/**************** pagedir_init ****************/
bool valid_dir(const char *pageDirectory) 
{
  char filename[strlen(pageDirectory) + 1];
  strcpy(filename, pageDirectory);
  strcat(filename, "/.crawler"); 

  FILE* fp;
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "*** Not a valid directory: %s\n", pageDirectory);
    return false;
  }
  else {
    fclose(fp);
    return true;
  }
}

/**************** pagedir_save() ****************/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) 
{
  // determine number of digits in the ID
  int digits = 1;
  int idIncrement = docID;
  while (idIncrement >= 10) {
    digits = digits + 1;
    idIncrement = idIncrement / 10;
  }

  // allocate space for pageDirectory and id characters
  char* filename = mem_malloc(strlen(pageDirectory) + digits * sizeof(char) + 2);
  sprintf(filename,"%s/%d", pageDirectory, docID);
  
  // create file for writing
  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "*** Could not open file\n");
    fclose(fp);
    mem_free(filename);
    return;
  }

  //print URL, depth, webpage contents
  fprintf(fp, "%s\n", webpage_getURL(page));
  fprintf(fp, "%d\n", webpage_getDepth(page));
  fprintf(fp, "%s", webpage_getHTML(page));
  
  fclose(fp);
  mem_free(filename);
}

/**************** getPage() ****************/
webpage_t*
page_get(const char* pageDirectory, const int docID) 
{
  // determine number of digits in the ID
  int digits = 1;
  int idIncrement = docID;
  while (idIncrement >= 10) {
    digits = digits + 1;
    idIncrement = idIncrement / 10;
  }

  // allocate space for pageDirectory and id characters
  char* filename = mem_malloc(strlen(pageDirectory) + digits * sizeof(char) + 2);
  sprintf(filename,"%s/%d", pageDirectory, docID);
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    //fclose(fp);
    mem_free(filename);
    return NULL;
  }
  //read in the url, depth, and html from the file
  char *url = file_readLine(fp);
  int depth = fscanf(fp, "%d\n", &depth);
  char *html = file_readFile(fp);

  //save into a new page
  webpage_t *page = webpage_new(url, depth, html);
  //set pointers to NULL
  url = NULL;
  html = NULL;

  //free file and filename
  fclose(fp);
  mem_free(filename);
  return page; 
}
