#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
#include "protocol.hpp"

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
        handlerHttp();
    }
    void handlerHttp()
    {
        std::string buffer;
        while (true)
        {
            std::cout << "Please input# ";
            std::string line;
            getline(std::cin, line);
            std::stringstream ss(line);
            int lhs, rhs;
            char op;
            ss >> lhs >> op >> rhs;
            Request req(lhs, rhs, op);
            std::string req_str;
            req.Serialize(&req_str);
            std::string package = enLength(req_str);
            send(sockfd_, package.c_str(), package.size(), 0);
            package = "";
            if (!recvPackage(sockfd_, buffer, &package))
            {
                LogMessage(NORMAL, "A complete message was not received");
            }
            std::string res_str = deLength(package);
            Response res;
            if (!res.Parse(res_str))
            {
                LogMessage(NORMAL, "res.Parse fail");
            }
            std::cout << "exit_code: " << res.exit_code_ << std::endl
                      << "result: " << res.result_ << std::endl;
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