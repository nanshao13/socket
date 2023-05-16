#pragma once

#include <cassert>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEV "/dev/null"

void daemonSelf(const char *path = nullptr)
{
    // 忽略掉异常信号
    signal(SIGPIPE, SIG_IGN);
    // 让子进程调用setsid，因为setsid的调用者不能是process group leader
    if (fork() > 0)
        exit(0);
    pid_t id = setsid();
    assert(id != -1);
    // 守护进程（孤儿进程）是脱离终端的，所以要将0、1、2文件关闭/重定向
    int fd = open(DEV, O_WRONLY);
    if (fd != -1) // 优先重定向，可以防止因为向关闭的文件写入而导致中断
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }
    // 可选选项：切换路径
    if (path)
        chdir(path);
}