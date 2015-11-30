#include <stdint.h>
#include <stdbool.h>
#include "lib/bitvector.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "board-ht.h"
#include "lib/queue.h"
#include "lib/boardutil.h"

bitvector press_button(bitvector B, int row, int col, uint8_t width, uint8_t height) {
  ASSERT(row < height);
  ASSERT(col < width);
  if (width * height != 1) {
    bool at_edge_u = 0;
    bool at_edge_l = 0;
    bool at_edge_d = 0;
    bool at_edge_r = 0;
    if (row == 0) at_edge_u = 1;
    if (col == 0) at_edge_l = 1;
    if (row == (height - 1)) at_edge_d = 1;
    if (col == (width - 1)) at_edge_r = 1;

    if (at_edge_l && at_edge_u) {
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
    } else if (at_edge_r && at_edge_u) {
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
    } else if (at_edge_l && at_edge_d) {
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
      B = bitvector_flip(B, get_index((row-1), col, width, height));
    } else if (at_edge_r && at_edge_d) {
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
      B = bitvector_flip(B, get_index((row-1), col, width, height));
    } else if (at_edge_l) {
      ASSERT(!at_edge_u && !at_edge_d);
      B = bitvector_flip(B, get_index((row-1), col, width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
    } else if (at_edge_r) {
      ASSERT(!at_edge_u && !at_edge_d);
      B = bitvector_flip(B, get_index((row-1), col, width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
    } else if (at_edge_u) {
      ASSERT(!at_edge_l && !at_edge_r);
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
    } else if (at_edge_d) {
      ASSERT(!at_edge_l && !at_edge_r);
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
      B = bitvector_flip(B, get_index((row-1), col, width, height));
    } else {
      ASSERT(!at_edge_r && !at_edge_l && !at_edge_u && !at_edge_d);
      B = bitvector_flip(B, get_index(row, (col-1), width, height));
      B = bitvector_flip(B, get_index(row, (col+1), width, height));
      B = bitvector_flip(B, get_index((row-1), col, width, height));
      B = bitvector_flip(B, get_index((row+1), col, width, height));
    }
  }
  B = bitvector_flip(B, get_index(row, col, width, height));
  return B;
}

void print_solution(struct board_data* B, uint8_t width, uint8_t height) {
  if (B->back_ptr == NULL) return;
  print_solution(B->back_ptr, width, height);
  printf("%d:%d \n", B->row, B->col);
  return;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: lightsout <board name>\n");
    return 1;
  }

  char* board_filename = argv[1];

  uint8_t* width = xcalloc(1, sizeof(uint8_t));
  uint8_t* height = xcalloc(1, sizeof(uint8_t));
  bitvector* bitvec_ptr = xcalloc(1, sizeof(bitvector));
  *bitvec_ptr = bitvector_new();
  bitvector solved = bitvector_new();
  if (file_read(board_filename, bitvec_ptr, width, height) == 0) {
    fprintf(stderr, "Board file does not exist in given path \n");
    free(bitvec_ptr);
    free(width);
    free(height);
    return 1;
  }
  if (*bitvec_ptr == solved) {
    free(bitvec_ptr);
    free(width);
    free(height);
    printf("board solved \n");
    return 0;
  }
  queue_t Q = queue_new();
  struct board_data* board = xcalloc(1, sizeof(struct board_data));
  board->board = *bitvec_ptr;
  board->back_ptr = NULL;
  free(bitvec_ptr);
  enq(Q, (void*) board);
  hset_t H = ht_new(30);
  ht_insert(H, board);

  while (!queue_empty(Q)) {
    struct board_data*  B = (struct board_data*) deq(Q);
    for (int row = 0; row < *height; row++) {
      for (int col = 0; col < *width; col++) {
        bitvector newboard = press_button(B->board, row, col, *width, *height);

        if (newboard == solved) {
          struct board_data* new_board = xcalloc(1, sizeof(struct board_data));
          new_board->board = newboard;
          new_board->back_ptr = B;
          new_board->row = row;
          new_board->col = col;
          enq(Q, (void*) new_board);
          ht_insert(H, new_board);
          print_solution(new_board, *width, *height);
          queue_free(Q, NULL);
          hset_free(H);
          free(width);
          free(height);
          printf("board solved \n");
          return 0;
        }

        if (ht_lookup(H, newboard) == NULL) {
          struct board_data* new_board = xcalloc(1, sizeof(struct board_data));
          new_board->board = newboard;
          new_board->back_ptr = B;
          new_board->row = row;
          new_board->col = col;
          enq(Q, (void*) new_board);
          ht_insert(H, new_board);
        }
      }
    }
  }
  free(width);
  free(height);
  queue_free(Q, NULL);
  hset_free(H);
  fprintf(stderr, "Board is not solvable \n");
  return 1;
}

