#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	int	pid;
	int	fd;
	printf("About to run who into a file\n");

	if((pid=fork())==-1){
		perror("fork");
		exit(1);
	}
	
	//child process
	if(pid==0){
		close(1);
		fd=open("userlist.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
		execlp("who","who",NULL);
		perror("exec");
		exit(1);
	}
	//parent process
	if(pid!=0){
		wait(NULL);
		printf("Done running who. result in userlist.\n");
	}
}
