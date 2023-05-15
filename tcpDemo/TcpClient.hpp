#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"

enum
{
    CREATE_SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    UES_ERROR
};

class TcpClient
{
public:
    TcpClient(const std::string &ip, const uint16_t &port)
        : sockfd_(-1), ip_(ip), port_(port)
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ == -1)
        {
            LogMessage(FATAL, "create socket error");
            exit(CREATE_SOCKET_ERROR);
        }
        LogMessage(NORMAL, "create socket success");
    }
    void run()
    {
        for (;;)
        {
            struct sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = inet_addr(ip_.c_str());
            serverAddress.sin_port = htons(port_);
            if (connect(sockfd_, (struct sockaddr *)&serverAddress, sizeof serverAddress) == -1)
            {
                LogMessage(FATAL, "connect error");
                exit(CREATE_SOCKET_ERROR);
            }
            LogMessage(NORMAL, "connect success");
            serviceIO();
        }
    }
    void serviceIO()
    {
        for (;;)
        {
            std::cout << "Please input# ";
            std::string in;
            getline(std::cin, in);
            write(sockfd_, in.c_str(), in.size());
            char buf[1024];
            ssize_t n = read(sockfd_, buf, sizeof(buf) - 1);
            if (n > 0)
            {
                buf[n] = 0;
                std::cout << "read# " << buf << std::endl;
            }
        }
    }
    ~TcpClient() 
    {
        if (sockfd_ != -1)
        {
            close(sockfd_);
        }
    }

private:
    int sockfd_;
    std::string ip_;
    uint16_t port_;
};