#ifndef _DGRAM_H
#define _DGRAM_H
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int make_dgram_server_socket(int);
int get_internet_address(char *,int,int *,struct sockaddr_in *);
int make_dgram_client_socket();
int make_internet_address(char *,int,struct sockaddr_in *);

#endif
