#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

enum
{
    USAGE_ERROR = 1,
    SOCKET_ERROR,
    PTHREAD_CREATE_ERROR
};

class udpClient
{
public:
    udpClient(const std::string &serverip, const uint16_t &serverport)
        : sockfd_(-1), serverip_(serverip), serverport_(serverport), reader_(-1)
    {
    }
    void init()
    {
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ == -1)
        {
            std::cerr << "socker error" << errno << std::endl;
            exit(SOCKET_ERROR);
        }
        // 客户端只需要一个port即可但不需要确定是哪个，而且如果用户显式bind了一个固定的端口，有可能会发生端口冲突的问题（有可能其它进程在用该端口）
        // 所以不需要用户显式bind，OS会自动bind，会自动生成一个端口并且不会和其它进程占用的端口冲突
    }
    static void *readMessage(void *args)
    {
        int sockfd = *(static_cast<int *>(args));
        pthread_detach(pthread_self());
        while (true)
        {
            char buf[1024];
            struct sockaddr_in src_addr;
            bzero(&src_addr, sizeof src_addr);
            socklen_t addrlen = sizeof src_addr;
            ssize_t n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&src_addr, &addrlen);
            if (n >= 0)
                buf[n] = 0;
            std::string message = buf;
            std::cout << message << std::endl;
        }
        return nullptr;
    }
    void start()
    {
        int n = pthread_create(&reader_, nullptr, readMessage, (void *)&sockfd_);
        if (n != 0)
        {
            std::cerr << "pthread_create error" << errno << std::endl;
            exit(PTHREAD_CREATE_ERROR);
        }
        struct sockaddr_in dest_addr;
        bzero(&dest_addr, sizeof dest_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(serverport_);
        dest_addr.sin_addr.s_addr = inet_addr(serverip_.c_str());
        std::string message;
        char buf[1024];
        while (true)
        {
            std::cout << "Please Enter# ";
            fgets(buf, sizeof buf, stdin);
            buf[strlen(buf) - 1] = 0;
            message = buf;
            sendto(sockfd_, message.c_str(), message.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            usleep(100);
        }
    }
    ~udpClient()
    {
    }

private:
    int sockfd_;
    std::string serverip_;
    uint16_t serverport_;
    pthread_t reader_;
};