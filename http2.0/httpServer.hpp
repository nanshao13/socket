#pragma once
#include <iostream>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "protocol.hpp"
#include "util.hpp"

const static int backlog = 5;
using func_t = std::function<void(const Request &, Response *)>;

class HttpServer
{
public:
    HttpServer(uint16_t port, func_t func)
        : listen_sockfd_(-1), port_(port), func_(func)
    {
        listen_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_sockfd_ == -1)
        {
            std::cout << "create listen sockfd error" << std::endl;
            exit(1);
        }
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port_);
        if (bind(listen_sockfd_, (struct sockaddr *)&local, sizeof local) == -1)
        {
            std::cout << "bind error" << std::endl;
            exit(1);
        }
        if (listen(listen_sockfd_, backlog) == -1)
        {
            std::cout << "listen error" << std::endl;
            exit(1);
        }
    }
    void Run()
    {
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t peer_length;
            int sockfd = accept(listen_sockfd_, (struct sockaddr *)&peer, &peer_length);
            if (sockfd == -1)
            {
                std::cout << "accept error" << std::endl;
                continue;
            }
            pid_t pid = fork();
            if (pid == 0)
            {
                close(listen_sockfd_);
                if (fork() > 0)
                    exit(0);
                HttpHandler(sockfd);
                close(sockfd);
                exit(0);
            }
            close(sockfd);
            int status = 0;
            if (waitpid(pid, &status, 0) == -1)
            {
                std::cout << "waitpid error" << std::endl;
                exit(1);
            }
            std::cout << "exit code  : " << ((status >> 8) & 0xFF) << std::endl
                      << "exit signal: " << (status & 0x7F) << std::endl;
        }
    }
    void HttpHandler(int sockfd)
    {
        char buffer[4096];
        while (true)
        {
            ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                std::cout << "------------------http request start------------------" << std::endl;
                std::cout << buffer;
                std::cout << "------------------http request  end ------------------" << std::endl;
                Request request;
                if (!request.Parse(buffer))
                {
                    std::cout << "request.Parse error" << std::endl;
                    exit(1);
                }
                Response response;
                func_(request, &response);
                std::string send_string;
                if (!response.Serialize(&send_string))
                {
                    std::cout << "response.Serialize error" << std::endl;
                    exit(1);
                }
                //为什么想输出到显示器必须要加endl？
                send(sockfd, send_string.c_str(), send_string.size(), 0);
            }
            else if (n == 0)// 为什么偶尔是无效的呢？
            {
                std::cout << "disconnect a connection, "
                          << "sockfd[" << sockfd << "]closed, " 
                          << "process[" << getpid() << "]exit" << std::endl;
                close(sockfd);
                exit(0);
            }
        }
    }
    
    ~HttpServer() {}

private:
    int listen_sockfd_;
    uint16_t port_;
    func_t func_;
};