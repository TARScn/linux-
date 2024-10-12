## unix的进程控制
### 目录
- [常用函数](#常用函数)
- [控制逻辑](#控制逻辑)
- [代码解读](#代码解读)
### 常用函数
- execvp()

`int execvp(const char *file, char *const argv[]);`

清除当前的程序内存并进入指定的程序。

__传入参数__：

**_file_**:执行的程序名或程序路径，程序名则根据PATH环境查找执行程序。

__*argv*__:执行程序参数，第一个为程序名。如："ls" "-l" "/home"

__返回__：成功不返回，执行出错返回-1。

- fork()
  
`pid_t fork(void);`

在父进程下创建一个子进程，但是会先执行完父进程再进入子进程。所以需要适时使用wait()。

__返回__：子进程的pid。

- wait()
  
`pid_t wait(int *status);`

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/71a521338a84c114b2ecae4ac8f5cd6ec7677124/readme_picture/wait.png" alt="wait流程" width="50%" height="50%">

当父进程调用 wait() 时，它会被阻塞，直到其一个子进程结束执行。wait() 会回收子进程的资源，并通过 status 参数传递子进程的退出状态给父进程。这样可以确保系统资源得到正确释放，防止僵尸进程的产生。

__传入参数__：

**_status_**:指向一个整数的指针，用于存储子进程的退出状态信息。可以为 NULL，表示不关心子进程的退出状态。

__返回__：成功: 返回结束的子进程的进程ID(PID),失败: 返回 -1。

- exit()
  
`void exit(int status);`

exit()用于终止当前进程的执行并返回一个状态码给操作系统。

__传入参数__：status: 一个整数值，表示程序的退出状态。通常：

EXIT_SUCCESS（通常为 0）：表示程序成功完成。

EXIT_FAILURE（通常为 1）：表示程序以失败状态终止。

也可以使用其他自定义的整数值来表示不同的退出原因。

### 控制逻辑

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/b4e3ade077b743002a863264ff763dbd7edff489/readme_picture/psh2.png" alt="控制逻辑" width="50%" height="50%">

### 代码解读
```
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
```
__execute__

创建子进程：调用 fork()。

pid == -1：创建子进程失败，输出错误信息并退出程序。

pid == 0：在子进程中执行命令。调用 execvp 以执行 arglist[0] 指定的命令，并传递参数列表 arglist。

如果 execvp 失败，输出错误信息并退出子进程。

pid > 0：在父进程中等待子进程结束。

使用 wait 循环等待特定的子进程结束，获取其退出状态。

输出子进程的退出状态。

__运行结果__
```
[lcx@localhost codes]$ ./psh2 
Arg[0]?: ls
Arg[1]?: -l
Arg[2]?: /home
Arg[3]?: 
total 4
drwx------. 19 lcx   lcx   4096 Oct 12 11:07 lcx
drwx------.  3 mysql mysql   78 Oct  6 18:14 mysql
child exited with status 0
```
