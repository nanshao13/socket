#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const static std::string defaultIp = "0.0.0.0";

enum
{
    USAGE_ERROR = 1,
    SOCKET_ERROR,
    BIND_ERROR,
    IFSTREAM_ERROR
};

class udpServer
{
public:
    using func_t = std::function<void(int, int16_t, std::string, std::string)>;

    udpServer(const func_t &callback, const int16_t &serverport, const std::string &serverip = defaultIp)
        : sockfd_(-1), callback_(callback), serverport_(serverport), serverip_(serverip)
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
        struct sockaddr_in local;
        bzero(&local, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(serverport_);
        // local.sin_addr.s_addr = inet_addr(ip_.c_str());// 服务端不会绑定指定IP，因为不知道客户端访问的是该服务端的哪个IP（公网、内网、环回. . .）
        local.sin_addr.s_addr = htonl(INADDR_ANY); // 服务端的真实写法，INADDR_ANY本质就是0，即绑定IP为0，即客户端无论访问该服务端的哪个IP，服务端都会接受（对应端口）
        int n = bind(sockfd_, (struct sockaddr *)&local, sizeof(local));
        if (n == -1)
        {
            std::cerr << "bind error: " << errno << " : " << strerror(errno) << std::endl;
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
            ssize_t n = recvfrom(sockfd_, buf, sizeof(buf) - 1, 0, (sockaddr *)&src_addr, &addrlen);
            if (n > 0)
            {
                buf[n] = '\0';
                int16_t clientport = ntohs(src_addr.sin_port);
                std::string clientip = inet_ntoa(src_addr.sin_addr);
                std::cout << clientip << "[" << clientport << "]# " << buf << std::endl;
                callback_(sockfd_, clientport, clientip, buf);
            }
        }
    }
    ~udpServer()
    {
    }

private:
    int sockfd_;
    int16_t serverport_;
    std::string serverip_;
    func_t callback_;
};
