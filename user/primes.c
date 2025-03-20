#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(int listen) {
    int prime = 0;
    int pass_name = 0;
    int pipes[2];
    int forked = 0; //表示是否创建子进程

    while (1) {
        int read_b = read(listen, &pass_name, 4);

        //没有读到数字
        if (read_b == 0) {
            close(listen); // 关闭父亲的读的那端

            //如果是父亲 关闭写端不在给子进程写入
            if (forked) {
                close(pipes[1]);
                int child;
                wait(&child);
            }
            exit(0);
        }

        // 打印质数
        // 由于线性筛质数所以拿到的第一个一定是质数
        if (prime == 0) {
            prime = pass_name;
            printf("prime %d\n", prime);
        }

        //将自己作为最小质因数筛质数
        if (pass_name % prime != 0) {
            if (!forked) {
                pipe(pipes);
                forked = 1;
                int ret = fork();
                if (ret == 0) {
                    // 关闭子进程用不到的设备符号
                    close(pipes[1]);
                    close(listen);
                    run(pipes[0]);
                } else {
                    close(pipes[0]);
                }
            }
            write(pipes[1], &pass_name, 4);
        }

    }

}

int main(int argc, char *argv[]) {
    int pipes[2];
    pipe(pipes);

    for (int i = 2; i <= 35; i++) {
        write(pipes[1], &i, 4);
    }
    close(pipes[1]);
    run(pipes[0]);
    exit(0);
}