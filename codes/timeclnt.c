#include <stdio.h>          // 标准输入输出函数
#include <stdlib.h>         // 标准库函数，如exit()
#include <sys/types.h>      // 数据类型定义
#include <sys/socket.h>     // 套接字函数和数据结构
#include <netinet/in.h>     // Internet地址族
#include <netdb.h>          // 网络数据库操作，如gethostbyname()
#include <string.h>         // 字符串操作函数

#define oops(m) { perror(m); exit(1); }    // 定义错误处理宏

int main(int ac, char *av[]) {
    struct sockaddr_in servadd;    // 服务器地址结构
    struct hostent *hp;            // 主机信息结构
    int sock_id, sock_fd;          // 套接字描述符
    char message[BUFSIZ];          // 消息缓冲区
    int messlen;                   // 消息长度

    // 检查命令行参数是否正确
    if (ac != 3) {
        fprintf(stderr, "Usage: %s hostname port\n", av[0]);
        exit(1);
    }

    // 创建一个TCP套接字
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1)
        oops("socket");

    // 清零服务器地址结构，确保所有字段初始化为0
    memset(&servadd, 0, sizeof(servadd));

    // 根据主机名获取主机信息
    hp = gethostbyname(av[1]);
    if (hp == NULL)
        oops(av[1]);

    // 将主机的IP地址复制到服务器地址结构中
    memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);

    // 设置服务器端口号，转换为网络字节序
    servadd.sin_port = htons(atoi(av[2]));

    // 设置地址族为IPv4
    servadd.sin_family = AF_INET;

    // 连接到服务器
    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect");

    // 从服务器读取消息，最多读取BUFSIZ字节
    messlen = read(sock_id, message, BUFSIZ);
    if (messlen == -1)
        oops("read");

    // 将接收到的消息写入标准输出（文件描述符1）
    if (write(1, message, messlen) != messlen)
        oops("write");

    // 关闭套接字，释放资源
    close(sock_id);

    return 0;
}
