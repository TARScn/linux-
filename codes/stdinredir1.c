#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
	int fd;
	char line[100];

	fgets(line,100,stdin);printf("%s\n",line);

	close(0);

	if((fd=open("/etc/passwd",O_RDONLY))!=0){
		fprintf(stderr,"can't open passwd\n");
		exit(1);
	}

	fgets(line,100,stdin);printf("%s\n",line);
}
