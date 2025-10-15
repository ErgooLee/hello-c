
#include <unistd.h>
#include <stdio.h>
//基于内核缓冲区，单向，父子进程之间。
int main() {
    int fd[2];
    pipe(fd); // 创建管道 fd[0] 读端，fd[1] 写端

    if (fork() == 0) {
        // 子进程
        close(fd[1]);  // 关闭写端
        char buf[100];
        read(fd[0], buf, sizeof(buf));
        printf("Child received: %s\n", buf);
    } else {
        // 父进程
        close(fd[0]);  // 关闭读端
        write(fd[1], "Hello pipe!", 12);
    }
}
