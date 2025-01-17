#include "dgram.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define oops(m,x) {perror(m);exit(x);}

int main(int ac,char *av[])
{
	int sock;
	char *msg;
	struct sockaddr_in saddr;

	if(ac!=4){
		fprintf(stderr,"usage:dgsend host port 'message'\n");
		exit(1);
	}

	msg=av[3];
	if((sock=make_dgram_client_socket())==-1)
		oops("can't make socket",2);
	make_internet_address(av[1],atoi(av[2]),&saddr);

	if(sendto(sock,msg,strlen(msg),0,(struct sockaddr *)&saddr,sizeof(saddr))==-1)
		oops("sendto failed",3);
	return 0;
}

