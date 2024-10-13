#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int	pid;
	int	fd;
	printf("About to run who into a file\n");

	if((pid=fork())==-1){
		perror("fork");
		exit(1);
	}
	if(pid==0){
		
	}
}
