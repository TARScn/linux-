#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define INPUTLEN 10

void inthandler();

int main()
{
	struct sigaction newhandler;
	sigset_t blocked;
	char x[INPUTLEN];

	newhandler.sa_handler = inthandler;
	newhandler.sa_flags = SA_RESTART|SA_RESETHAND; //设置重置模式

	sigemptyset(&blocked);
	sigaddset(&blocked,SIGQUIT);  //加入SIGQUIT,执行INT时忽略QUIT
	newhandler.sa_mask = blocked; //设置

	if(sigaction(SIGINT,&newhandler,NULL)==-1) perror("sigaction");
	else
		while(1){
			fgets(x,INPUTLEN,stdin);
			printf("input:%s",x);
		}
}

void inthandler()
{
	printf("Called with signal %d\n",s);
	sleep(3);
	printf("done handling signal %d\n",s);
}

