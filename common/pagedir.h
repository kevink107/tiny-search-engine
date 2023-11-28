/* 
 * pagedir.h - header file for CS50 pagedir.c file
 *
 * Includes functions used to check whether a directory exists or not, save a directory,
 * and get a webpage based on the document ID.
 *
 * Kevin King
 * CS50 TSE
 * Winter 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"


/**************** functions ****************/

/**************** pagedir_init ****************/
/* Constructs the pathname for the .crawler file in that directory and opens the file for writing.
 * 
 * We return:
 *    true - if directory exists and is writeable
 *    false - if directory doesn't exist or isn't writeable
 * Parameters: 
 *    *pageDirectory - the existing directory to write downloaded webpages
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_init ****************/
/* Constructs the pathname for the .crawler file in that directory and opens the file for reading.
 * 
 * We return:
 *    true - if directory exists and is readable
 *    false - if directory doesn't exist or isn't readable
 * Parameters: 
 *    *pageDirectory - the existing directory to write downloaded webpages
 */
bool valid_dir(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Constructs the pathname for the page file in pageDirectory and opens the file for writing
 * 
 * We do:
 *    saves a webpage too a file with the URL on the first line, depth on the second, 
 *    and HTML on the rest. The filename is the ID number provided.
 * Parameters: 
 *    *page - webpage to obtain content from
 *    *pagDirectory - the existing directory to write downloaded webpages
 *    docID - unique ID number of a filename
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/**************** getPage() ****************/
/* Gets a page file from the directory based on the document ID that's given
 *
 * We return: 
 *    the webpage that correlates to the specific pageDirectory/docID 
 * Parameters:
 *    *pagDirectory - the existing directory to write downloaded webpages
 *    docID - unique ID number of a filename
 */
webpage_t* page_get(const char* pageDirectory, const int docID);

#endif