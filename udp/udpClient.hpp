#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

enum
{
    USAGE_ERROR = 1,
    SOCKET_ERROR,
};

class udpClient
{
public:
    udpClient(const std::string &serverip, const uint16_t &serverport)
        : sockfd_(-1), serverip_(serverip), serverport_(serverport)
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
    void start()
    {
        struct sockaddr_in dest_addr;
        bzero(&dest_addr, sizeof dest_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(serverport_);
        dest_addr.sin_addr.s_addr = inet_addr(serverip_.c_str());
        std::string message;
        while (true)
        {
            std::cout << "Please Enter# ";
            std::cin >> message;
            ssize_t n = sendto(sockfd_, (void *)message.c_str(), message.size(), 0, (sockaddr *)&dest_addr, sizeof(dest_addr));
        }
    }
    ~udpClient()
    {
    }

private:
    int sockfd_;
    std::string serverip_;
    uint16_t serverport_;
};