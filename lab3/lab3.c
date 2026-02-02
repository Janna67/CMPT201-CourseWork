#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // for ssize_t

#define HISTORY_SIZE 5

static char *my_strdup(const char *s) {
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (!p)
    return NULL;
  memcpy(p, s, n);
  return p;
}

static void free_history(char *history[], int size) {
  for (int i = 0; i < size; i++) {
    free(history[i]);
    history[i] = NULL;
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  int count = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
      // EOF (Ctrl+D) or error
      break;
    }

    // strip trailing '\n'
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    // store (circular)
    int idx = count % HISTORY_SIZE;
    free(history[idx]);
    history[idx] = my_strdup(line);
    if (!history[idx]) {
      perror("malloc");
      break;
    }
    count++;

    // command: print
    if (strcmp(line, "print") == 0) {
      int stored = (count < HISTORY_SIZE) ? count : HISTORY_SIZE;
      int start = count - stored;

      for (int i = start; i < count; i++) {
        printf("%s\n", history[i % HISTORY_SIZE]);
      }
    }
  }

  free(line);
  free_history(history, HISTORY_SIZE);
  return 0;
}
