#include <stdbool.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"

//#if BITVECTOR_LIMIT > 32
//  ASSERT(BITVECTOR_LIMIT <= 64);
//  typedef uint64_t bitvector;
//#elif  16 < BITVECTOR_LIMIT && BITVECTOR_LIMIT <= 32
//  typedef uint32_t bitvector;
//#elif  8 < BITVECTOR_LIMIT &&  BITVECTOR_LIMIT <= 16
//  typedef uint16_t bitvector;
//#elif  0 < BITVECTOR_LIMIT && BITVECTOR_LIMIT <= 8
//  typedef uint8_t bitvector;
//#endif

bitvector bitvector_new() {
  bitvector new_bitvector = 0;
  return new_bitvector;
}

bool bitvector_get(bitvector B, uint8_t i) {
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector tester = 1;
  tester = tester << i;
  bitvector result = tester & B;
  return result == tester;
}

bitvector bitvector_flip(bitvector B, uint8_t i) {
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector tester = 1;
  tester = tester << i;
  if ((tester & B) > 0) {
    B = B - tester;
  } else {
    ASSERT((tester & B) == 0);
    B = B + tester;
  }
  return B;
}

bool bitvector_equal(bitvector B1, bitvector B2){
  bitvector comparevec = B1 & B2;
  return comparevec == B1 && comparevec == B2;
}

