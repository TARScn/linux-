#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define CHILD_MESS  "I want a cookie\n"
#define PAP_MESS    "testing..\n"
#define OOPS(m,x)   { perror(m); exit(x); }

volatile sig_atomic_t stop = 0;

// 信号处理函数，用于捕捉 SIGINT (Ctrl+C) 信号
void handle_sigint(int sig) {
    stop = 1;
}

int main()
{
    int pipefd[2];
    ssize_t len;
    char buf[BUFSIZ];
    ssize_t read_len;

    // 注册信号处理函数
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        OOPS("signal", 1);
    }

    if(pipe(pipefd) == -1)
        OOPS("can't get a pipe", 1);

    switch(fork()){
        case -1:
            OOPS("can't fork", 2);
        case 0:
            // Child process
            close(pipefd[0]); // 关闭不需要的读取端

            len = strlen(CHILD_MESS);
            while(!stop){
                if(write(pipefd[1], CHILD_MESS, len) != len)
                    OOPS("write", 3);
                sleep(5);
            }

            close(pipefd[1]); // 关闭写入端
            exit(0);
        default:
            // Parent process
            close(pipefd[1]); // 关闭不需要的写入端

            while(!stop){
                // 父进程不再写入管道，而是仅从管道读取
                read_len = read(pipefd[0], buf, BUFSIZ);
                if(read_len == -1){
                    perror("read");
                    break;
                }
                if(read_len == 0){
                    // 管道已关闭
                    break;
                }
                // 输出从子进程读取到的消息
                if(write(STDOUT_FILENO, buf, read_len) == -1){
                    perror("write to stdout");
                    break;
                }

                // 父进程也可以定期写入自己的消息，如果需要
                // 这里为了避免混淆，暂时不写入
                // 如果需要，可以创建另一个管道或使用其他机制
            }

            close(pipefd[0]); // 关闭读取端

            // 等待子进程结束
            wait(NULL);

            printf("Parent exiting.\n");
    }

    return 0;
}
