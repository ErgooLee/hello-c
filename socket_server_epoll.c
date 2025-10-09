//
// Created by chao.li14 on 2025/10/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAX_EVENTS 10
#define PORT 8888

// 设置非阻塞
int set_non_blocking(const int fd) {
    const int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return 0;
}

int main() {
    const int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    listen(listen_fd, 10);
    set_non_blocking(listen_fd);

    int epoll_fd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    printf("Server listening on port %d\n", PORT);

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        const int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listen_fd) {
                // 接受新连接
                const int conn_fd = accept(listen_fd, NULL, NULL);
                set_non_blocking(conn_fd);
                ev.events = EPOLLIN | EPOLLET; // ET 模式
                ev.data.fd = conn_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
                printf("New connection: %d\n", conn_fd);
            } else {
                // 处理客户端数据
                char buf[512];
                const long n = read(events[i].data.fd, buf, sizeof(buf));
                if (n <= 0) {
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                    printf("close connection %d\n", events[i].data.fd);
                } else {
                    buf[n] = '\0';
                    printf("Received: %s from %d \n", buf, events[i].data.fd);
                    write(events[i].data.fd, buf, n); // 回显
                }
            }
        }
    }
    close(listen_fd);
    return 0;
}
