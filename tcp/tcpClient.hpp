#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const static int buf_num = 1024;

enum ERROR
{
    CREATE_SOCKET_ERROR = 1,
    CONNECT_ERROR
};

class tcpClient
{
public:
    tcpClient(const std::string &server_ip, const uint16_t &server_port);
    void start();
    void hander();
    ~tcpClient();

private:
    int client_socket_;
    std::string server_ip_;
    uint16_t server_port_;
};

inline tcpClient::tcpClient(const std::string &server_ip, const uint16_t &server_port)
    : client_socket_(-1), server_ip_(server_ip), server_port_(server_port)
{
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ == -1)
    {
        std::cerr << "create socket error" << std::endl;
        exit(CREATE_SOCKET_ERROR);
    }
    std::cerr << "create socket success" << std::endl;
}

inline void tcpClient::start()
{
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip_.c_str());
    server.sin_port = htons(server_port_);
    if (connect(client_socket_, (struct sockaddr *)&server, sizeof server) == -1)
    {
        std::cerr << "connect error" << std::endl;
        exit(CONNECT_ERROR);
    }
    std::cerr << "connect success" << std::endl;
    hander();
}

inline void tcpClient::hander()
{
    for (;;)
    {
        std::cout << "Enter input# ";
        std::string message;
        getline(std::cin, message);
        write(client_socket_, message.c_str(), message.size());
        char buf[buf_num];
        ssize_t n = read(client_socket_, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = 0;
            std::cout << buf << std::endl;
        }
        else
        {
            break;
        }
    }
}

inline tcpClient::~tcpClient()
{
    if (client_socket_ >= 0)
    {
        close(client_socket_);
    }
}