#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "Log.hpp"
#include "Thread.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"

enum
{
    CREATE_SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    UES_ERROR
};

const static int backlog = 5;

class TcpServer;
class ThreadData
{
public:
    TcpServer *ts_;
    int sockfd_;
    ThreadData(TcpServer *ts, int sockfd)
        : ts_(ts), sockfd_(sockfd)
    {
    }
    ~ThreadData() {}
};

class TcpServer
{
public:
    TcpServer(const uint16_t &port)
        : sockfd_(-1), port_(port)
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ == -1)
        {
            LogMessage(FATAL, "create socket error");
            exit(CREATE_SOCKET_ERROR);
        }
        LogMessage(NORMAL, "create socket success");
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port_);
        if (bind(sockfd_, (struct sockaddr *)&local, sizeof local) == -1)
        {
            LogMessage(FATAL, "bind error");
            exit(BIND_ERROR);
        }
        LogMessage(NORMAL, "bind success");
        if (listen(sockfd_, backlog) == -1)
        {
            LogMessage(FATAL, "listen error");
            exit(LISTEN_ERROR);
        }
        LogMessage(NORMAL, "listen success");
    }
    void run()
    {
        for (;;)
        {
            struct sockaddr_in clientAddress;
            socklen_t clientLength = sizeof clientAddress;
            int sockfd = accept(sockfd_, (struct sockaddr *)&clientAddress, &clientLength);
            if (sockfd == -1)
            {
                LogMessage(NORMAL, "accept error");
                continue;
            }
            LogMessage(NORMAL, "accept success");
            /**
             * version 1
             * 只能够同时进行一个通话
             */
            // serviceIO(sockfd);

            /**
             * version 2
             * 多进程版
             */
            // pid_t pid = fork();
            // if (pid == 0)
            // {
            //     close(sockfd_);// 关闭多余的fd
            //     serviceIO(sockfd);
            //     exit(0);
            // }
            // close(sockfd);
            // // int status = 0;
            // // waitpid(pid, &status, 0);// 阻塞等待和轮询等待都不合适
            // 多进程会造成很大的资源消耗，这里还是选多线程方案更合适

            /**
             * version 3
             * 进程池版
             */
            // 多进程会造成很大的资源消耗，这里还是选多线程方案更合适

            /**
             * version 4
             * 多线程版
             */
            // ThreadData *td = new ThreadData(this, sockfd);
            // Thread td(start_routine, td);
            // td.join();// 会导致主线程阻塞

            /**
             * version 5
             * 线程池版
             */
            ThreadPool<Task>::getInstance(10)->enqueue(Task(serviceIO, sockfd));
        }
    }
    /**
     * version 4
     * 多线程版
     */
    // static void *start_routine(void *args)
    // {
    //     pthread_detach(pthread_self());
    //     ThreadData *td = static_cast<ThreadData *>(args);
    //     serviceIO(td->sockfd_);
    //     delete td;
    //     pthread_exit(nullptr);
    // }
    ~TcpServer()
    {
        if (sockfd_ != -1)
        {
            close(sockfd_);
        }
    }

private:
    int sockfd_;
    uint16_t port_;
};