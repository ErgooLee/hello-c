//
// Created by chao.li14 on 2025/10/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 5

int main() {
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t client_len;
    ssize_t bytes_read;

    // 1. 创建 socket
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 绑定所有网卡
    server_addr.sin_port = htons(PORT);

    // 3. 绑定 socket 到端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. 开始监听
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. 接受客户端连接
    client_len = sizeof(client_addr);
    const int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // 6. 接收数据
    while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);

        // 7. 回发响应
        const char *reply = "Message received by server.";
        send(client_fd, reply, strlen(reply), 0);
    }

    if (bytes_read == -1)
        perror("recv failed");

    // 8. 关闭 socket
    close(client_fd);
    close(server_fd);
    printf("Server shut down.\n");

    return 0;
}
