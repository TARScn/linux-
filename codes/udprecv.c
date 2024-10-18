#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFSIZE 1024  // 缓冲区大小
#define PORT 4444     // 服务器监听的端口号

int main() {
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    ssize_t recv_len;

    // 创建 UDP 套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // 使用 IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // 监听所有可用的网络接口
    server_addr.sin_port = htons(PORT); // 设置监听端口号

    // 绑定套接字到端口
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP 服务器正在监听端口 %d...\n", PORT);

    // 监听并接收来自客户端的消息
    client_len = sizeof(client_addr);
    while (1) {
        memset(buffer, 0, BUFSIZE); // 清空缓冲区
        recv_len = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            break;
        }

        // 打印收到的消息
        printf("收到消息: %s\n", buffer);

        // 打印客户端的 IP 地址和端口号
        printf("来自客户端的 IP 地址: %s, 端口号: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    // 关闭套接字
    close(sockfd);
    return 0;
}
