#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    struct sockaddr_in server_addr;
    char buffer[1024];

    // 1. 创建 socket
    const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    // 3. 连接服务器
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // 4. 发送消息
    const char *msg = "Hello from client!";
    send(socket_fd, msg, strlen(msg), 0);
    printf("Sent: %s\n", msg);

    // 5. 接收响应
    const ssize_t bytes_read = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
    } else if (bytes_read == 0) {
        printf("Server closed connection.\n");
    } else {
        perror("recv failed");
    }

    // 6. 关闭连接
    close(socket_fd);
    printf("Client closed.\n");

    return 0;
}
