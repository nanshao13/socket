#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const static int backlog = 32;

class Sock {
public:
    static int Socket() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cout << "socket error" << std::endl;
            exit(1);
        }
        std::cout << "socket success" << std::endl;
        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt);
        return sockfd;
    }
    static void Bind(int sockfd, uint16_t port) {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);
        if (bind(sockfd, (struct sockaddr *)&local, sizeof local) == -1) {
            std::cout << "bind error" << std::endl;
            exit(2);
        }
        std::cout << "bind success" << std::endl;
    }
    static void Listen(int sockfd) {
        if (listen(sockfd, backlog) == -1) {
            std::cout << "listen error" << std::endl;
            exit(3);
        }
        std::cout << "listen success" << std::endl;
    }
    static int Accept(int listen_sockfd, std::string *ip, uint16_t *port) {
        struct sockaddr_in peer;
        socklen_t peerlen;
        return accept(listen_sockfd, (struct sockaddr *)&peer, &peerlen);
    }
};