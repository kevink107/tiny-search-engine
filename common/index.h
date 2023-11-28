/* 
 * index.h - header file for CS50 index.c file
 *
 * An *index* is a hashtable with word keys and counter sets as items. Each of
 * the items are identified by their page directory document IDs.
 *
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"

#ifndef __INDEX_H
#define __INDEX_H

/**************** global variables ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new() ****************/
/* Creates a new (empty) index.
 * 
 * Caller provides:
 *    number of slots to be used for the index (must be > 0)
 * We return:
 *    pointer to the new index; return NULL if error
 * We guarantee:
 *    index is initialized empty
 * Caller is responsible for:
 *    later calling index_delete
 */
index_t* index_new(int num_slots);

/**************** index_insert() ****************/
/* Insert counters set, identified by word and docID, into the given index
 * 
 * Caller provides:
 *    valid pointer to index, valid string for word, valid integer for docID
 * We return:
 *    true, if word was inserted correctly into the index
 *    false, if word was not inserted correctly
 */
bool index_insert(index_t* index, char* word, int docID);

/**************** index_save() ****************/
/* Saves the indexer file 
 * 
 * Caller provides:
 *    valid pointer to index and a valid file open for writing
 * We do:
 *    iterate through the index and print each word and its (docID, count)
 *    pairs to the index file
 */
void index_save(index_t* index, FILE* fp);

/**************** index_delete() ****************/
/* Delete index, calling a delete function on each item
 * 
 * Caller provides:
 *    valid index pointer
 * We do:
 *    if index==NULL, do nothing
 *    calls hashtable_delete function with helper delete function on each item
 */
void index_delete(index_t* index);

/**************** index_get() ****************/
/* Gets the index based on the index file given
 * 
 * Caller provides:
 *    valid file open for reading
 * We return:
 *    scan the file indexFile for each word and each of the (docID, count)
 *    pairs that correlate to that word, returning the index 
 */
index_t* index_get(FILE* indexFile);

#endif