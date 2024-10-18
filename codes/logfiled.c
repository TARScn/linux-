#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define MSGLEN 512
#define oops(m,x) {perror(m);exit(x);}
#define SOCKNAME "/"

