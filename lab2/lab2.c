#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char buf[1024];

  while (1) {
    printf("Enter programs to run.\n> ");
    fflush(stdout);

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      break;
    }
    buf[strcspn(buf, "\n")] = '\0';

    if (buf[0] == '\0') {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      execl(buf, buf, (char *)NULL);

      printf("Exec failure\n");
      fflush(stdout);
      _exit(127);
    } else {
      int status;
      if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
      }
    }
  }

  return 0;
}
