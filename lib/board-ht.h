/*
 * Hash tables wrapper
 *
 * 15-122 Principles of Imperative Computation */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "bitvector.h"
#include "hset.h"

#ifndef _BOARD_HASHTABLE_H_
#define _BOARD_HASHTABLE_H_

struct board_data {
  uint8_t test;
  bitvector board;
  struct board_data* back_ptr;
};

/* IMPORTANT: You must respect the hash set interface in hset.h when
 * writing your implementation of these functions! */

/* Initializes a new hash table with the given capacity */
hset_t ht_new(size_t capacity)
  /*@requires capacity > 0; @*/
  /*@ensures \result != NULL; @*/ ;

/* ht_lookup(H,B) returns
 * NULL if no struct containing the board B exists in H
 * A struct containing the board B if one exists in H.
 */
struct board_data *ht_lookup(hset_t H, bitvector B)
  /*@requires H != NULL; @*/ ;

/* ht_insert(H,e) has no return value, because it should have as
 * a precondition that no struct currently in the hashtable contains
 * the same board as DAT->board.
 */
void ht_insert(hset_t H, struct board_data *DAT)
  /*@requires H != NULL; @*/ ;

#endif
