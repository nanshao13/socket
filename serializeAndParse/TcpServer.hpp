#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "Log.hpp"
#include "protocol.hpp"

enum
{
    CREATE_SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    UES_ERROR
};

const static int backlog = 5;

using func_t = std::function<bool(const Request &, Response *)>;

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
    void run(func_t task)
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
            pid_t pid = fork();
            if (pid == 0)
            {
                close(sockfd_);
                if (fork() > 0)
                    exit(0);
                handlerHttp(sockfd, task);
                close(sockfd);
                exit(0);
            }
            int status = 0;
            if (waitpid(pid, &status, 0) == -1)
            {
                LogMessage(FATAL, "waitpid error");
            }
            LogMessage(NORMAL, "waitpid success, exit_code[%d], exit_signal[%d]", (status >> 8) & 0xFF, status & 0x7F);
        }
    }
    void handlerHttp(int sockfd, func_t task)
    {
        std::string buffer;
        for (;;)
        {
            //1. 接收Package
            std::string package;
            if (!recvPackage(sockfd, buffer, &package))
            {
                LogMessage(NORMAL, "A complete message was not received");
            }
            //2. 解析Request
            std::string req_str = deLength(package);
            Request req;
            if (!req.Parse(req_str))
            {
                LogMessage(NORMAL, "req.Parse fail");
            }
            //3. 用Request填充Response
            Response res;
            task(req, &res);
            //4. 序列化Response
            std::string res_str;
            if (!res.Serialize(&res_str))
            {
                LogMessage(NORMAL, "res.Serialize fail");
            }
            package = "";
            package = enLength(res_str);
            //5. 发送Response
            send(sockfd, package.c_str(), package.size(), 0);
        }
    }
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