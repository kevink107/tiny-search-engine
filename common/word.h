/* 
 * word.h - header file for CS50 word.c file
 *
 * Includes a function that normalizes a word.
 *
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */


#ifndef __WORD_H
#define __WORD_H

#include <stdlib.h>
#include <ctype.h>

/**************** functions ****************/

/**************** normalize_word() ****************/
/* Normalizes a given character string. 
 *
 * We return:
 *    pointer to the normalized character "word" string
 * Parameters:
 *    *word - character string that hasn't been normalized yet
 */
char* normalize_word(char* word);

#endif