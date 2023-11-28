/* 
 * hashtable.c - CS50 'set' module
 *
 * see hashtable.h for more information.
 *
 * Kevin King, January 26, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "hashtable.h"
#include "mem.h"
#include "set.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
  set_t** slot;
  int num_slots;       
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* none */

/**************** hashtable_new() ****************/
/* see hashtable.h for description */
hashtable_t* 
hashtable_new(const int num_slots) 
{
  if (num_slots < 1) {
    return NULL;
  }
  // memory allocation for new hashtable
  hashtable_t* ht = (hashtable_t*) mem_malloc(sizeof(hashtable_t));

  if (ht == NULL) {
    mem_free(ht);
    return NULL;
  }

  // allocate memory for the number of slots given, where each slot 
  // takes up the size of a struct set 
  ht->slot = mem_calloc(num_slots, sizeof(struct set_t*));
  if (ht->slot == NULL) {
    mem_free(ht->slot);
    return NULL;
  }
  
  ht->num_slots = num_slots;
  for (int i = 0; i < num_slots; i++) {
    // initialize each hashtable slot as a new empty set
    ht->slot[i] = set_new();
  }
  return ht;
}

/**************** hashtable_insert() ****************/
/* see hashtable.h for description */
bool 
hashtable_insert(hashtable_t* ht, const char* key, void* item) 
{
  if (ht == NULL || key == NULL || item == NULL || hashtable_find(ht, key) != NULL) {
    return false;
  }
  else {
    // use hash function to find the index for the key to map to
    int index = hash_jenkins(key, ht->num_slots);
    if (ht->slot[index] != NULL) {
      if (set_find(ht->slot[index], key) != NULL) {
        return false;
      }
      // calls upon set_insert to add the item into the slot that 
      // the key maps to 
      return(set_insert(ht->slot[index], key, item));
    }
    return false;
  }
}

/**************** hashtable_find() ****************/
/* see hashtable.h for description */
void* 
hashtable_find(hashtable_t* ht, const char* key)
{
  if (ht == NULL || key == NULL || ht->slot == NULL) {
    return NULL;
  }
  else {
    for (int i = 0; i < ht->num_slots; i++) {
      int index = hash_jenkins(key, ht->num_slots);
      // finds the slot index of the given key, if it exists
      if (set_find(ht->slot[index], key) != NULL) {
        // calls set_find to return item
        return set_find(ht->slot[index], key);
      }
    }
    return NULL;
  }
}

/**************** hashtable_print() ****************/
/* see hashtable.h for description */
void 
hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item))
{
  if (fp != NULL) {
    if (ht != NULL) {
      for (int i = 0; i < ht->num_slots; i++) {
        // call set_print which uses itemprint function 
        // on each item in the hashtable
        if ((*itemprint) != NULL) {
          set_print(ht->slot[i], fp, itemprint);
        }
        // new line for each slot in the hashtable
        else {
          fputc('\n', fp);
        }
        fputc('\n', fp);
      }
    }
    else {
      fputs("(null)", fp);
    }
  }
}

/**************** hashtable_iterate() ****************/
/* see hashtable.h for description */
void 
hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
  if (ht != NULL && itemfunc != NULL) {
    for (int i = 0; i < ht->num_slots; i++) {
      // call set_iterate which uses itemfunc on each item
      set_iterate(ht->slot[i], arg, itemfunc);
    }
  }
}

/**************** hashtable_delete() ****************/
/* see hashtable.h for description */
void 
hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) ) 
{
  if (ht != NULL) {
    for (int i = 0; i < ht->num_slots; i++) {
      // calls set_delete which uses itemdelete function
      // on each of the slots
      set_delete(ht->slot[i], itemdelete);
    }
    // free all slots and the hashtable
    mem_free(ht->slot);
    mem_free(ht);
    ht = NULL;
  }
  #ifdef MEMTEST
    mem_report(stdout, "End of set_delete");
  #endif  
}