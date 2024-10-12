#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAXARGS  20
#define ARGLEN   100

char *makestring(char *);
void execute(char **);

int main()
{
    char *arglist[MAXARGS+1];
    int numargs = 0;
    char argbuf[ARGLEN];
    
    while(numargs < MAXARGS){
        printf("Arg[%d]?: ", numargs);
        if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
            arglist[numargs++] = makestring(argbuf);
        else{
            if(numargs > 0){
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}

void execute(char **arglist)
{
    pid_t pid;
    int status;

    pid = fork();
    if(pid == -1){
        perror("fork");
        exit(1);
    }
    if(pid == 0){
        // 子进程
        execvp(arglist[0], arglist);
        perror("execvp");
        exit(1);
    }
    // 父进程
    if(waitpid(pid, &status, 0) == -1){
        perror("waitpid");
        exit(1);
    }
    if(WIFEXITED(status)){
        printf("child exited with status %d\n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
        printf("child terminated by signal %d\n", WTERMSIG(status));
    }

    // 释放内存
    for(int i = 0; arglist[i] != NULL; i++){
        free(arglist[i]);
    }
}

char *makestring(char *buf)
{
    char *cp;
    size_t len = strlen(buf);

    if(len > 0 && buf[len-1] == '\n')
        buf[len-1] = '\0';
    else
        buf[len] = '\0'; // 确保字符串终止

    cp = malloc(len + 1);
    if(cp == NULL){
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
