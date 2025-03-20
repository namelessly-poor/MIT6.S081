#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid;
    int pipe1[2], pipe2[2];
    char buf[] = {'a'};

    // 为管道读写端分配文件标识符
    pipe(pipe1);
    pipe(pipe2);

    int ret = fork();
    // 进入子进程
    if (ret == 0) {
        pid = getpid(); // get the process id
        close(pipe1[1]);
        // close(pipe2[0]);
        read(pipe1[0], buf, 1);
        printf("%d: received ping\n", pid);
        write(pipe2[1], buf, 1);
        exit(0);
    } else {
        pid = getpid();
        // close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], buf, 1);
        read(pipe2[0], buf, 1);
        printf("%d: received pong\n", pid);
        exit(0);
    }
    
}