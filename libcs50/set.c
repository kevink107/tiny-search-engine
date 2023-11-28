/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Kevin King, January 21, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  char* key;
  void* item;                 // pointer to data for this item
  struct setnode *next;
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;       // head of the list of key:item pairs in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t* setnode_new(const char* key, void* item);

/**************** set_new() ****************/
/* see set.h for description */
set_t*
set_new(void)
{
  set_t* set = (set_t*) mem_malloc(sizeof(set_t));

  if (set == NULL) {
    // if memory allocation error, free set
    mem_free(set);
    return NULL;
  } 
  else {
    set->head = NULL;
    return set;
  }
}

/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t*
setnode_new(const char* key, void* item)
{
  // memory allocation for new node
  setnode_t* node = (setnode_t*) mem_malloc(sizeof(setnode_t));

  if (node == NULL) {
    return NULL;
  }
  else {
    // allocates memory for the key - based on how long it is
    node->key = (char*) mem_malloc(strlen(key)+1);
    if (node->key == NULL) {
      mem_free(node);
      return NULL;
    }
    // put string key into node->key
    strcpy(node->key, key);
    // initialize and set next node to NULL
    node->next = NULL;
    node->item = item;
    return node;
  }

}

/**************** set_insert() ****************/
/* see set.h for description */
bool
set_insert(set_t* set, const char* key, void* item) 
{
  if (set == NULL || key == NULL || item == NULL) {
    return NULL;
  }
  else {
    // loop through each setnode in the set to find if key exists
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      if (strcmp(key, node->key) == 0) {
        return false;
      }
    }
    // create new setnode if key doesn't already exist
    setnode_t* new = setnode_new(key, item);
    if (new == NULL) {
      return false;
    }
    new->next = set->head;
    set->head = new;
  }
  return true;

  #ifdef MEMTEST
    mem_report(stdout, "After set_insert");
  #endif
}

/**************** set_find() ****************/
/* see set.h for description */
void* 
set_find(set_t* set, const char* key)
{
  if (set == NULL || key == NULL) {
    return NULL;
  }
  else {
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      // if key is in set, return item 
      if (strcmp(key, node->key) == 0) {
        return node->item;
      }
    }
  }
  
  return NULL;
}

/**************** set_print() ****************/
/* see set.h for description */
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t* node = set->head; node != NULL; node=node->next) {
        // print node 
        if (itemprint != NULL) {
          // print node's item
          (*itemprint)(fp, node->key, node->item);
          // comma-separated list of nodes
          if (node->next != NULL) {
            fputc(',', fp);
          }
        }
      }
      fputc('}',fp);
    }
    else {
      fputs("(null)", fp);
    }
  }
}

/**************** set_iterate() ****************/
/* see set.h for description */
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item);
    }
  }
}

/**************** set_delete() ****************/
/* see set.h for description */
void set_delete(set_t* set, void (*itemdelete)(void* item) )
{
  if (set != NULL) {
    for (setnode_t* node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {
        (*itemdelete)(node->item);
      }
      // this increments the loop to the next node
      setnode_t* next = node->next;
      // free all nodes and keys
      mem_free(node->key);
      mem_free(node);
      node = next;
    }
    mem_free(set);
  }

#ifdef MEMTEST
  mem_report(stdout, "End of set_delete");
#endif
}
