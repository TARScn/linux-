#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFSIZE 4096
#define COPYMODE 0644  //设定复制后文件的执行权限

void oops(char *, char *);

int main(int ac, char *av[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFSIZE];

    if (ac != 3)
    {
        fprintf(stderr, "Usage: %s source destination\n", av[0]);
        exit(1);
    }

    // 打开源文件
    if ((in_fd = open(av[1], O_RDONLY)) == -1)
        oops("Can't open", av[1]);

    // 创建目标文件，使用 open 而非 creat，便于扩展
     if ((out_fd = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, COPYMODE)) == -1)
        oops("Can't create", av[2]);

    // 读取源文件并写入目标文件
    while ((n_chars = read(in_fd, buf, BUFFSIZE)) > 0)
    {
        if (write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to", av[2]);
    }

    if (n_chars == -1)
        oops("Read error from", av[1]);

    // 关闭文件描述符并检查错误
    if (close(in_fd) == -1)
        oops("Close error on input file", av[1]);
    if (close(out_fd) == -1)
        oops("Close error on output file", av[2]);

    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "ERROR: %s ", s1);
    perror(s2);
    exit(1);
}
