#include "dgram.h"

#define HOSTLEN 256

int make_internet_address(char *hostname, int port, struct sockaddr_in *addrp)
{
    memset(addrp, 0, sizeof(*addrp));  // 清空地址结构
    
    if (hostname == NULL) {
        // 如果主机名为空，则监听所有可用接口
        addrp->sin_addr.s_addr = htonl(INADDR_ANY);  // 使用 INADDR_ANY 监听所有接口
    } else {
        // 否则根据主机名获取 IP 地址
        struct hostent *hp = gethostbyname(hostname);
        if (hp == NULL)
            return -1;
        printf("addr: %s\n", inet_ntoa(*(struct in_addr *)hp->h_addr));
        memcpy(&(addrp->sin_addr), hp->h_addr, hp->h_length);  // 复制 IP 地址
    }

    addrp->sin_port = htons(port);  // 设置端口号
    addrp->sin_family = AF_INET;    // 设置地址族为 IPv4
    return 0;
}

int make_dgram_server_socket(int portnum)
{
    struct sockaddr_in saddr;
    char hostname[HOSTLEN];
    int sock_id;

    sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_id == -1)
        return -1;

    gethostname(hostname, HOSTLEN);
	printf("Current hostname: %s\n", hostname);
    if (make_internet_address(NULL, portnum, &saddr) == -1)
        return -1;

    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
        perror("bind error");
        return -1;
    }

    return sock_id;
}

int get_internet_address(char *host, int len, int *portp, struct sockaddr_in *addrp)  
{
    strncpy(host, inet_ntoa(addrp->sin_addr), len);  
    *portp = ntohs(addrp->sin_port);
    return 0;
}

int make_dgram_client_socket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}
