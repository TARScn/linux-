#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main()
{
	int fd;
	int newfd;
	char line[100];
	
	fgets(line,100,stdin);
	printf("%s",line);

	fd=open("/etc/passwd",O_RDONLY);
	printf("now open file is %d\n",fd);

	newfd = dup2(fd,0);	//关闭输出0，并将文件重定向到输出0
	if(newfd!=0){
		perror("can't duplicate fd to 0\n");
		exit(1);
	}
	close(fd);
	fgets(line,100,stdin);
	printf("%s",line);

}

