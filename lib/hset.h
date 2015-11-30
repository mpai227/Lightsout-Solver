/*
 * Hash sets interface
 * 
 * 15-122 Principles of Imperative Computation */

#include <stdlib.h>
#include <stdbool.h>

#ifndef _HSET_H_
#define _HSET_H_

typedef void *hset_elem;
typedef bool elem_equal_fn(hset_elem x, hset_elem y);
typedef size_t elem_hash_fn(hset_elem x);
typedef void elem_free_fn(hset_elem x);

typedef struct hset_header* hset_t;

hset_t hset_new(size_t capacity,
                elem_equal_fn *elem_equal,
                elem_hash_fn *elem_hash,
                elem_free_fn *elem_free)
  /*@requires capacity > 0; @*/ 
  /*@requires elem_equal != NULL && elem_hash != NULL; @*/
  /* if elem_free is NULL, then elements will not be freed */
  /*@ensures \result != NULL; @*/ ;

/* If insertion removes an existing element from the set, that removed
 * element  will be  returned  so  that the  client  can  free it,  if
 * necessary */
hset_elem hset_insert(hset_t H, hset_elem x)
  /*@requires H != NULL; @*/ ;

hset_elem hset_lookup(hset_t H, hset_elem x)
  /*@requires H != NULL; @*/ ;

void hset_free(hset_t H)
  /*@requires H != NULL; @*/ ;

#endif
