#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define INPUTLEN 100

void inthandler(int);
void quithandler(int);

int main()
{
	char input[INPUTLEN];
	int nchars;

	signal(SIGINT,inthandler);
	signal(SIGQUIT,quithandler);

	do{
		printf("\nType a message\n");
		if((nchars=read(0,input,(INPUTLEN-1)))==-1) 
			perror("read returned and error");
		else{
			input[nchars]='\0';
			printf("You typed %s\n",input);
		}
	}while(strncmp(input,"quit",4)!=0);
}

void inthandler(int s)
{
	printf("Received signal %d..waiting\n",s);
	sleep(2);
	printf("Leaving inthandler.\n");
}

void quithandler(int s)
{
	printf("Received signal %d..waiting\n",s);
	sleep(2);
	printf("Leaving quithandler.\n");
}
