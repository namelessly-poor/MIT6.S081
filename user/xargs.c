#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define buf_size 512

int main(int argc, char *argv[]) {

    char stdIn[512];
    int size = read(0, stdIn, sizeof stdIn);

    int i = 0, j = 0;
    int line = 0;
    for (int k = 0; k < size; k++) {
        if (stdIn[k] == '\n') {
            ++line;
        }
    }

    char output[line][64];
    for (int k = 0; k < size; k++) {
        output[i][j++] = stdIn[k];
        if (stdIn[k] == '\n') {
            output[i][j - 1] = 0;
            ++i;
            j = 0;
        }
    }

    char *arguments[MAXARG];
    for (j = 0; j < argc - 1; j++) {
        arguments[j] = argv[1 + j];
    }
    i = 0;
    while (i < line) {
        arguments[j] = output[i++];
        if (fork() == 0) {
            exec(argv[1], arguments);
            exit(0);
        }
        wait(0);
    }
    exit(0);
}