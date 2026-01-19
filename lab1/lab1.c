#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *buf = NULL;
    size_t cap = 0;

    while (1) {
        printf("Please enter some text: ");
        fflush(stdout);

        ssize_t nread = getline(&buf, &cap, stdin);
        if (nread == -1) {
            putchar('\n');
            break;
        }

        // Remove trailing newline for nicer output
        if (nread > 0 && buf[nread - 1] == '\n') {
            buf[nread - 1] = '\0';
            nread--;
        }

        // Exit if user just pressed Enter
        if (nread == 0) {
            break;
        }

        printf("Tokens:\n");

        char *saveptr = NULL;
        char *tok = strtok_r(buf, " ", &saveptr);
        while (tok != NULL) {
            printf("  %s\n", tok);
            tok = strtok_r(NULL, " ", &saveptr);
        }
    }

    free(buf);
    return 0;
}
