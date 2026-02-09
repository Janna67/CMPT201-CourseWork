#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEAP_GROW 256
#define BLOCK_SIZE 128
#define BUF_SIZE 128

extern void *sbrk(intptr_t increment);

struct header {
  uint64_t size;
  struct header *next;
};

static void handle_error(const char *msg) {
  perror(msg);
  _exit(1);
}

static void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len;

  if (data_size == sizeof(uint64_t)) {
    len = snprintf(buf, BUF_SIZE, format, *(uint64_t *)data);
  } else {
    len = snprintf(buf, BUF_SIZE, format, *(void **)data);
  }

  if (len < 0)
    handle_error("snprintf");
  if (write(STDOUT_FILENO, buf, (size_t)len) < 0)
    handle_error("write");
}

int main(void) {
  void *heap_start = sbrk(HEAP_GROW);
  if (heap_start == (void *)-1)
    handle_error("sbrk");

  struct header *block1 = (struct header *)heap_start;
  struct header *block2 = (struct header *)((char *)heap_start + BLOCK_SIZE);

  block1->size = BLOCK_SIZE;
  block1->next = NULL;

  block2->size = BLOCK_SIZE;
  block2->next = block1;

  size_t data_size = BLOCK_SIZE - sizeof(struct header);
  unsigned char *b1_data =
      (unsigned char *)((char *)block1 + sizeof(struct header));
  unsigned char *b2_data =
      (unsigned char *)((char *)block2 + sizeof(struct header));

  memset(b1_data, 0, data_size);
  memset(b2_data, 1, data_size);

  print_out("first block:       %p\n", &block1, sizeof(block1));
  print_out("second block:      %p\n", &block2, sizeof(block2));

  print_out("first block size:  %lu\n", &block1->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &block1->next, sizeof(void *));
  print_out("second block size: %lu\n", &block2->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &block2->next, sizeof(void *));

  for (size_t i = 0; i < data_size; i++) {
    char line[4];
    int n = snprintf(line, sizeof(line), "%u\n", (unsigned)b1_data[i]);
    if (n < 0)
      handle_error("snprintf");
    if (write(STDOUT_FILENO, line, (size_t)n) < 0)
      handle_error("write");
  }

  for (size_t i = 0; i < data_size; i++) {
    char line[4];
    int n = snprintf(line, sizeof(line), "%u\n", (unsigned)b2_data[i]);
    if (n < 0)
      handle_error("snprintf");
    if (write(STDOUT_FILENO, line, (size_t)n) < 0)
      handle_error("write");
  }

  return 0;
}
