#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // First block with block->size >= size
  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      return cur->id;
    }
    cur = cur->next;
  }
  return -1; // not found
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_fit_size = 0;

  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (best_fit_id == -1 || cur->size < best_fit_size) {
        best_fit_id = cur->id;
        best_fit_size = cur->size;
      }
    }
    cur = cur->next;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_fit_size = 0;

  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (worst_fit_id == -1 || cur->size > worst_fit_size) {
        worst_fit_id = cur->id;
        worst_fit_size = cur->size;
      }
    }
    cur = cur->next;
  }

  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
Part 2: Coalescing Contiguous Free Blocks (Pseudo-code)

Goal: When a block is freed, merge it with adjacent free blocks that are
contiguous in memory (physically next to each other).

Assume:
- Free list is sorted by address (increasing).
- Each free block has a header at its start.
- "end_address(block)" = (char*)block + sizeof(header) + block->size

Pseudo-code:

coalesce(free_list_head, newly_freed_block):
  1) Insert newly_freed_block into free list in address-sorted order.

  2) Let prev = NULL, cur = free_list_head
     while cur != NULL and cur != newly_freed_block:
        prev = cur
        cur = cur->next

  3) // Merge with next if contiguous
     next = cur->next
     if next != NULL and end_address(cur) == (char*)next:
        cur->size = cur->size + sizeof(header) + next->size
        cur->next = next->next

  4) // Merge with prev if contiguous
     if prev != NULL and end_address(prev) == (char*)cur:
        prev->size = prev->size + sizeof(header) + cur->size
        prev->next = cur->next

  5) return free_list_head
*/
