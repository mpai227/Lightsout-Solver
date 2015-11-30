#include <stdint.h>
#include <stdbool.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"
int main() {
  bitvector test = bitvector_new();
  test = 0x00000001;
  return 0;
}
