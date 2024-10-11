#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
    // 获取当前时间
    time_t current_time;
    if (time(&current_time) == ((time_t)-1)) {
        perror("time");
        exit(EXIT_FAILURE);
    }

    // 将当前时间转换为本地时间
    struct tm *local_time = localtime(&current_time);
    if (local_time == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    // 格式化时间字符串
    char time_str[100];
    if (strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time) == 0) {
        fprintf(stderr, "strftime returned 0");
        exit(EXIT_FAILURE);
    }

    // 输出当前日期和时间
    printf("当前日期和时间: %s\n", time_str);

    return 0;
}
