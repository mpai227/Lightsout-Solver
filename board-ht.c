#include <stdint.h>
#include <stdbool.h>
#include "board-ht.h"
#include "lib/bitvector.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"

size_t bitvector_hash_fn(void* data) {
  struct board_data* hash_data = (struct board_data*) data;
  bitvector bitvec = hash_data->board;
  size_t h = 0;
  for (int i = 0; i < BITVECTOR_LIMIT; i++) {
    h = h + bitvector_get(bitvec, i)*(i + 1);
  }
  return h;
}

void bitvector_free_fn(void* x) {
  struct board_data* data = (struct board_data*) x;
  free(data);
}

bool bitvector_equal_fn(void* x, void* y) {
  struct board_data* B1 = (struct board_data*) x;
  struct board_data* B2 = (struct board_data*) y;
  return bitvector_equal(B1->board, B2->board);
}

hset_t ht_new(size_t capacity) {
  REQUIRES(0 < capacity);
  hset_t new_hset = hset_new(capacity, &bitvector_equal_fn, &bitvector_hash_fn, &bitvector_free_fn);
  ENSURES(new_hset != NULL);
  return new_hset;
}

struct board_data* ht_lookup(hset_t H, bitvector B) {
  REQUIRES(H != NULL);
  struct board_data* lkup_data = xcalloc(1, sizeof(struct board_data));
  lkup_data->board = B;
  void*  lkup_elem = hset_lookup(H, (void*) lkup_data);
  free(lkup_data);
  return (struct board_data*) lkup_elem;
}

void ht_insert(hset_t H, struct board_data* DAT) {
  REQUIRES(H != NULL);
  REQUIRES(ht_lookup(H, DAT->board) == NULL);
  void* insert_elem = (void*) DAT;
  hset_insert(H, insert_elem);
}


