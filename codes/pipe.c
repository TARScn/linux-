#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define oops(m,x)	{perror(m);exit(x);}

int main(int ac,char *av[])
{
	int thepipe[2],newfd,pid;

	if(ac!=3){

		fprintf(stderr,"usage: pipe cmd1 cmd2\n");
		exit(1);
	}
	if(pipe(thepipe)==-1)
		oops("can't get a pipe",1);
	
	switch(fork()){
		case -1:
			//error
			oops("can't fork",2);
		case 0:
			//child_p
			close(thepipe[0]);	//close pipe to read
			if(dup2(thepipe[1],1)==-1)	//redirect stdin to wr_pipe
				oops("can't redirect stdout",4);
			close(thepipe[1]);
			execlp(av[1],av[1],NULL);
			oops(av[1],5);
		default:
			//paprent_p
			close(thepipe[1]);
			if(dup2(thepipe[0],0)==-1)
				oops("can't redirect stdin",3);
			close(thepipe[0]);
			execlp(av[2],av[2],NULL);
			oops(av[2],4);
	}
}
