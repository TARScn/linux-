#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST

void show_info(struct utmp *ut);

int main()
{
    struct utmp current_record; // 用于存储当前记录
    int utmpfd; // 文件描述符
    int reclen = sizeof(current_record); // 记录的长度等于 utmp 结构的大小

    // 打开 utmp 文件以读取
    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) // 打开文件，检查返回值
    {
        perror("Error opening utmp file"); // 打印错误信息
        exit(1); // 退出程序
    }

    // 循环读取 utmp 文件中的记录
    while (read(utmpfd, &current_record, reclen) == reclen) // 读取记录
    {
        show_info(&current_record); // 显示当前记录的信息
    }

    close(utmpfd); // 关闭文件描述符
    return 0; // 程序结束
}

// 显示 utmp 记录的信息
void show_info(struct utmp *ut)
{
    // 根据不同的记录类型显示信息
    switch (ut->ut_type)
    {
        case USER_PROCESS: // 用户登录
            printf("%-8.8s ", ut->ut_user);  // 打印用户名
            printf("%-8.8s ", ut->ut_line);  // 打印终端名
            printf("%-16.16s ", ut->ut_host); // 打印主机名

            // 打印登录时间，确保使用 time_t 类型
            time_t login_time = (time_t)ut->ut_time; // 转换为 time_t
            printf("%s", ctime(&login_time)); // 打印登录时间
            break;
        default:
            break; // 处理其他记录类型
    }
}
