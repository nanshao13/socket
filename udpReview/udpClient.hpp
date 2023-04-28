#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum ERROR 
{
    USAGE_ERROR = 1,
    SOCKET_ERROR,
};

class udpClient
{
public:
    udpClient(const uint16_t &serverport, const std::string &serverip)
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
    }
    void start()
    {
        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(serverport_);
        dest_addr.sin_addr.s_addr = inet_addr(serverip_.c_str());
        std::string message;
        while (true)
        {
            std::cout << "Please enter# ";
            std::cin >> message;
            sendto(sockfd_, message.c_str(), message.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        }
    }
    ~udpClient()
    {
    }
private:
    int sockfd_;
    uint16_t serverport_;
    std::string serverip_;
};
