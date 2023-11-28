/*
 * word.c - CS50 'word' module
 *
 * see word.h for more information
 * 
 * Kevin King
 * CS50 Lab 5
 * Winter 2022
 */

 #include <stdlib.h>
 #include <ctype.h>
 #include "word.h"

/**************** normalize_word() ****************/
 char*
 normalize_word(char* word)
 {
   if (word == NULL) {
     return NULL;
   }
   else {
     // read word in its entirety
     for (char *i = word; *i != '\0'; i++) {
       // normalizes by making it lower case
       *i = tolower(*i);
     }
     return word;
   }
 }