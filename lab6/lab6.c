#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *make_message(const char *name) {
  size_t n = strlen("Hello, ") + strlen(name);
  char *msg = malloc(n + 1);
  if (!msg)
    return NULL;

  strcpy(msg, "Hello, ");
  strcat(msg, name);
  return msg;
}

int main(void) {
  const char *who = "CMPT201";
  char *msg = make_message(who);
  if (!msg) {
    fprintf(stderr, "malloc failed\n");
    return 1;
  }

  printf("%s\n", msg);
  free(msg);
  return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, msg) do {                                   \
    if (!(cond)) {                                                \
        fprintf(stderr, "ASSERT FAILED: %s\n", (msg));            \
        fprintf(stderr, "File: %s Line: %d\n", __FILE__, __LINE__); \
        exit(1);                                                  \
    }                                                             \
} while (0)

struct node {
    int value;
    struct node *next;
};

struct list_info {
    struct node *head;
    int sum;
    int count;
};

static struct node *push_front(struct node *head, int v) {
    struct node *n = malloc(sizeof(*n));
    if (!n) return NULL;
    n->value = v;
    n->next = head;
    return n;
}

static void free_list(struct node *head) {
    while (head) {
        struct node *tmp = head->next;
        free(head);
        head = tmp;
    }
}

static int sum_list(const struct node *head) {
    int total = 0;
    const struct node *cur = head;
    while (cur) {
        total += cur->value;
        cur = cur->next;
    }
    return total;
}

int main(void) {
    int data[] = { 10, 20, 30, 40, 50 };
    const int N = (int)(sizeof(data) / sizeof(data[0]));

    struct list_info info = {0};
    info.head = NULL;
    info.sum = 0;
    info.count = 0;

    for (int i = 0; i < N; i++) {
        info.head = push_front(info.head, data[i]);
        ASSERT(info.head != NULL, "malloc failed");
        info.count++;
    }

    struct node *cur = info.head;
    while (cur) {
        info.sum += cur->value;
        cur = cur->next;
    }

    ASSERT(info.count == N, "count mismatch");
    ASSERT(info.sum == sum_list(info.head), "sum mismatch");

    printf("count=%d, info.sum=%d\n", info.count, info.sum);

    free_list(info.head);
    return 0;
}
