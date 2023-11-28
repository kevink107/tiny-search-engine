/* 
 * counters.c - CS50 'set' module
 *
 * see counters.h for more information.
 *
 * Kevin King, January 24, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"
#include "mem.h"

/**************** local types ****************/
typedef struct countersnode {
  // do these need to be pointer ints?
  int key; 
  int counter;
  struct countersnode *next;
} countersnode_t;

/**************** global types ****************/
typedef struct counters {
  struct countersnode *head;
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static countersnode_t* countersnode_new(const int key);

/**************** counters_new() ****************/
/* see counters.h for description */
counters_t*
counters_new(void)
{
  counters_t* ctrs = (counters_t*) mem_malloc(sizeof(counters_t));

  if (ctrs == NULL) {
    // if null counterset, free it
    mem_free(ctrs);
    return NULL;
  }
  else {
    ctrs->head = NULL;
    return ctrs;
  }
}

/**************** countersnode_new ****************/
/* Allocate and initialize a countersnode */
static countersnode_t*
countersnode_new(const int key)
{
  // memory allocation for new node
  countersnode_t* node = (countersnode_t*) mem_malloc(sizeof(countersnode_t));

  if (node == NULL) {
    return NULL;
  }
  // set new node key and initialize counter to 1
  // initialize pointer to next node
  node->key = key;
  node->counter = 1;
  node->next = NULL;
  return node;
  
}

/**************** counters_add() ****************/
/* see counters.h for description */
int
counters_add(counters_t* ctrs, const int key) 
{
  if (ctrs == NULL || key < 0) {
    return 0;
  }
  
  // loop through all nodes in ctrs
  for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
    // increment counter if key already exists 
    if (node->key == key) {
      int prev_count = node->counter;
      node->counter = prev_count + 1;
      return node->counter;
    }
  }

  // if the key didn't exist already
  countersnode_t* new = countersnode_new(key);
  if (new != NULL) {
    // adds new node to the head of ctrs
    new->next = ctrs->head;
    ctrs->head = new;
  }
  return 0;

  #ifdef MEMTEST
    mem_report(stdout, "After counters_add");
  #endif
}

/**************** counters_get() ****************/
/* see counters.h for description */
int
counters_get(counters_t* ctrs, const int key) 
{
  if (ctrs == NULL || key < 0) {
    return 0;
  }

  // loop through all nodes in ctrs
  for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
    // if the key exists, return counter 
    if (node->key == key) {
      return node->counter;
    }
  }
  return 0;
} 

/**************** counters_set() ****************/
/* see counters.h for description */
bool counters_set(counters_t* ctrs, const int key, const int count) {
  if (ctrs != NULL && key >= 0 && count >= 0) {
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
      // if the key exists, change counter
      if (node->key == key) {
        node->counter = count;
        return true;
      }
    }
    // initialize new node if the key doesn't exist already
    countersnode_t* new = countersnode_new(key);
    if (new != NULL) {
      // sets new node's counter to the value provided
      new->counter = count;
      new->next = ctrs->head;
      ctrs->head = new;
      return true;
    }
  }
  return false;
}

/**************** counters_print() ****************/
/* see counters.h for description */
void counters_print(counters_t* ctrs, FILE* fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      // loop through each node to create comma-separated list
      for (countersnode_t* node = ctrs->head; node != NULL; node=node->next) {
        fprintf(fp, "%d=%d", node->key, node->counter);
        if (node->next != NULL) {
          fputc(',', fp);
        }
      }
      fputc('}', fp);
    }
    else {
      fputs("(null)", fp);
    }
  }
}

/**************** counters_iterate() ****************/
/* see counters.h for description */
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count))
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc on each item
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->counter);
    }
  }
}

/**************** counters_delete() ****************/
/* see counters.h for description */
void counters_delete(counters_t* ctrs)
{
  if (ctrs != NULL) {
    for (countersnode_t* node = ctrs->head; node != NULL; ) {
      countersnode_t* next = node->next;
      mem_free(node);
      // increments loop to next node
      node = next;
    }
    // free the counterset 
    mem_free(ctrs);
  }
  #ifdef MEMTEST
    mem_report(stdout, "End of counters_delete");
  #endif
}
