#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const static std::string defaultip = "0.0.0.0";
enum ERROR 
{
    USAGE_ERROR = 1,
    SOCKET_ERROR,
    BIND_ERROR
};

class udpServer
{
public:
    udpServer(const uint16_t &serverport, const std::string &serverip = defaultip)
        : sockfd_(-1), serverport_(serverport), serverip_(serverip)
    {
    }
    void init()
    {
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ == -1)
        {
            std::cerr << "socket " << errno << ": " << strerror(errno) << std::endl;
            exit(SOCKET_ERROR);            
        }
        std::cout << "create socket sucess: " << sockfd_ << std::endl;
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(serverport_); 
        local.sin_addr.s_addr = inet_addr(serverip_.c_str());
        int n = bind(sockfd_, (struct sockaddr *)&local, sizeof(local));
        if (n == -1)
        {
            std::cerr << "bind " << errno << ": " << strerror(errno) << std::endl;
            exit(BIND_ERROR);            
        }
    }
    void start()
    {
        char buf[1024];
        while (true)
        {
            struct sockaddr_in src_addr;
            socklen_t addrlen = sizeof(src_addr);
            ssize_t n = recvfrom(sockfd_, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&src_addr, &addrlen);
            if (n > 0)
            {
                buf[n] = 0;
                uint16_t clientport = ntohs(src_addr.sin_port);
                std::string clientip = inet_ntoa(src_addr.sin_addr);
                std::string message = (char *)buf;
                std::cout << clientip << " [" << clientport << "]# " << message << std::endl;
            }
        }
    }
    ~udpServer()
    {
    }
private:
    int sockfd_;
    uint16_t serverport_;
    std::string serverip_;
};
