#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.hpp"

const static int backlog = 5;
const static int buf_num = 1024;

enum ERROR
{
    CREATE_SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR
};

class tcpServer
{
public:
    tcpServer(const uint16_t &server_port);
    void start();
    void hander(int socket);
    ~tcpServer();

private:
    int server_socket_;
    uint16_t server_port_;
};

inline tcpServer::tcpServer(const uint16_t &server_port)
    : server_socket_(-1), server_port_(server_port)
{
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1)
    {
        logMessage(FATAL, "create socket error");
        exit(CREATE_SOCKET_ERROR);
    }
    logMessage(NORMAL, "create socket success");
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(server_port_);
    if (bind(server_socket_, (struct sockaddr *)&local, sizeof local) == -1)
    {
        logMessage(FATAL, "bind error");
        exit(BIND_ERROR);
    }
    logMessage(NORMAL, "bind success");
    if (listen(server_socket_, backlog) == -1)
    {
        logMessage(FATAL, "listen error");
        exit(LISTEN_ERROR);
    }
    logMessage(NORMAL, "listen success");
}

inline void tcpServer::start()
{
    for (;;)
    {
        struct sockaddr_in peer;
        socklen_t peer_len = sizeof peer;
        int socket = accept(server_socket_, (struct sockaddr *)&peer, &peer_len);
        if (socket == -1)
        {
            logMessage(ERROR, "accept error");
            continue;
        }
        logMessage(NORMAL, "accept success");
        hander(socket);
        close(socket);
    }
}

inline void tcpServer::hander(int socket)
{
    char buf[buf_num];
    for (;;)
    {
        ssize_t n = read(socket, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = 0;
            std::cout << "from client# " << buf << std::endl;
            std::string response = buf;
            response += "server[echo]";
            write(socket, response.c_str(), response.size());
        }
        else if (0 == n)
        {
            logMessage(NORMAL, "quit");
            break;
        }
    }
}

inline tcpServer::~tcpServer()
{
}