#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    // 定义存储管道的两个文件描述符，0读取，1写入
    int p1[2];
    int p2[2];
    char send[2];
    send[0] = 'a';
    send[1] = '\0';
    pipe(p1);
    pipe(p2);

    // if else 父子进程会并发执行
    if(fork() == 0) {
        if(read(p1[0], send, sizeof(send)) < 0) {
            fprintf(2, "read error\n");
        } else {
            fprintf(1, "%d: received ping\n", getpid());
        }
        close(p1[0]);
        close(p1[1]);

        write(p2[1], send, sizeof(send));
        close(p2[0]);
        close(p2[1]);
    } else {
        write(p1[1], send, sizeof(send));
        close(p1[0]);
        close(p1[1]);

        if(read(p2[0], send, sizeof(send)) < 0) {
            fprintf(2, "read error\n");
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(p2[0]);
        close(p2[1]);
    }

    exit(0);
}