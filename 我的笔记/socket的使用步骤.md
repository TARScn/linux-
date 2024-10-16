## scoket的使用步骤

### 目录
- [scoket的概念](#scoket的概念)
- [服务端代码](#服务端代码)
- [客户端代码](#客户端代码)
- [测试](#测试)
### scoket的概念

scoket可以视为两个主机进程之间通信的电线插槽。

通过主机IP和进程端口号进行数据的互传。用户在绑定端口时可以选择具体的协议和数据流方式，如IPv4、tcp等。

### 服务端代码

服务端程序收到客户端的消息后返回本地的时间。

```
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
```

__socket创建步骤__：

- 创建套接字

`sock_id = socket(AF_INET, SOCK_STREAM, 0);    // IPv4 字节流 TCP`

- 绑定套接字

通过sockaddr结构体设置套接字的IP和端口

`bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)`
  
- 监听套接字

`listen(sock_id, 1) //单次接受1 queue大小的数据`

- 接收套接字信号并作回复

`sock_fd = accept(sock_id, NULL, NULL)`

accept()返回的是一个文件描述符，所以可以使用fdopen()和write()，类似于文件操作一样写入数据。

### 客户端代码

客户端程序连接到服务端，显示收到的信息。

```
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
```

__socket连接步骤__：

- 创建套接字

`sock_id = socket(AF_INET, SOCK_STREAM, 0);`

- 设置套接字的IP和端口并连接

`connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)`

- 读取消息

`read(sock_id, message, BUFSIZ)`

### 测试

关闭服务端主机的防火墙
```
sudo firewall-cmd --permanent --add-port=1300/tcp
sudo firewall-cmd --reload
```

运行服务端程序
```
$ ./timeserv 
Server is listening on port 1300...
```
运行客户端程序，显示收到的服务端时间
```
$ ./timeclnt 192.168.81.133 1300
The time here is.. Wed Oct 16 09:38:21 2024
```
服务端显示收到连接
```
$ ./timeserv 
Server is listening on port 1300...
ojbk! got a call!
```
