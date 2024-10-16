#include <stdio.h>
#include <stdlib.h>         // 用于exit()
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>      // 用于inet_ntoa()

#define PORTNUM     1300
#define HOSTLEN     256
#define oops(m)     { perror(m); exit(1); }

int main(int ac, char *av[])  // 主函数
{
    struct sockaddr_in saddr;    
    struct hostent *hp;            // 主机信息结构
    char hostname[HOSTLEN];
    int sock_id, sock_fd;          // 套接字描述符
    FILE *sock_fp;                 
    time_t thetime;

    // 创建套接字
    sock_id = socket(AF_INET, SOCK_STREAM, 0);    // IPv4 字节流 TCP
    if (sock_id == -1)
        oops("socket");

    // 初始化saddr结构
    memset(&saddr, 0, sizeof(saddr));    // 清零
    // 设置地址族为IPv4
    saddr.sin_family = AF_INET;
    // 绑定到所有接口
    saddr.sin_addr.s_addr = INADDR_ANY;
    // 设置端口号，转换为网络字节序
    saddr.sin_port = htons(PORTNUM);

    // 绑定套接字到地址和端口
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        oops("bind");

    // 开始监听
    if (listen(sock_id, 1) != 0)
        oops("listen");

    printf("Server is listening on port %d...\n", PORTNUM);

    // 主循环
    while(1){
        sock_fd = accept(sock_id, NULL, NULL);
        if(sock_fd == -1)
            oops("accept");
        
        printf("ojbk! got a call!\n");
        
        // 将套接字描述符转换为 FILE*
        sock_fp = fdopen(sock_fd, "w");
        if(sock_fp == NULL)
            oops("fdopen");
        
        thetime = time(NULL);
        fprintf(sock_fp, "The time here is.. ");
        fprintf(sock_fp, "%s", ctime(&thetime));
        fclose(sock_fp); // 关闭 FILE* 同时关闭套接字描述符
    }

    // 关闭套接字（虽然在此代码中不会到达这里）
    close(sock_id);
    return 0;
}
