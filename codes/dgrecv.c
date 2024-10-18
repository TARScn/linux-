#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "dgram.h"

#define oops(m,x) {perror(m);exit(x);}

void say_who_called(struct sockaddr_in *);

int main(int ac,char *av[])
{
	int port;
	int sock;
	char buf[BUFSIZ];
	size_t msglen;
	struct sockaddr_in saddr;
	socklen_t saddrlen;

	if(ac==1||(port=atoi(av[1]))<=0)
		oops("usage:dgrecv portnum\n",1);
	if((sock=make_dgram_server_socket(port))==-1)
		oops("can't make socket",2);
	saddrlen=sizeof(saddr);
	while((msglen=recvfrom(sock,buf,BUFSIZ,0,(struct sockaddr *)&saddr,&saddrlen))>0){

		buf[msglen]='\0';
		printf("dgrecv:got a message:%s\n",buf);
		say_who_called(&saddr);
	}
	if(msglen==-1)
		perror("recvfrom error");
	return 0;
}

void say_who_called(struct sockaddr_in *addrp)
{
	char host[BUFSIZ];
	int port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("from:%s:%d\n",host,port);
}
