#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define DELAY 8

void child_code(int);
void parent_code(int);


int main()
{
	int newpid;
	printf("before:mypid is %d\n",getpid());
	if((newpid=fork())==-1) perror("fork");
	else if(newpid==0) child_code(DELAY);
	else parent_code(newpid);
}


void child_code(int delay)
{
	printf("child %d here, will sleep for %d seconds\n",getpid(),delay);
	sleep(delay);
	printf("child done, about to exit\n");
	exit(1);
}
void parent_code(int childpid)
{
	int wait_rv;
	int child_status;
	int high_8,low_7,bit_7;
	wait_rv=wait(&child_status);
	printf("done waiting for %d.Wait returned: %d\n",childpid,wait_rv);
	high_8=child_status>>8;
	low_7=child_status & 0x7f;
	bit_7=child_status & 0x80;
	printf("status:exit=%d, sig=%d, core=%d\n",high_8,low_7,bit_7);
}
