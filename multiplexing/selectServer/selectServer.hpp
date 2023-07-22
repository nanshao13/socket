#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/select.h>
#include "sock.hpp"

static const int fd_num = sizeof(fd_set) * 8;
static const int default_fd = -1;

class SelectServer {
public:
    SelectServer (uint16_t port) 
        : port_(port), listen_sockfd_(-1), fd_array_(nullptr)
    {}
    void init() {
        listen_sockfd_ = Sock::Socket();
        Sock::Bind(listen_sockfd_, port_);
        Sock::Listen(listen_sockfd_);
        fd_array_ = new int[fd_num];
        fd_array_[0] = listen_sockfd_;
        for (int i = 1; i < fd_num; ++i) {
            fd_array_[i] = default_fd;
        }
    }
    void run() {
        while (true) {
            fd_set rfds;
            FD_ZERO(&rfds);
            int max_fds = fd_array_[0];
            for (int i = 0; i < fd_num; ++i) {
                if (fd_array_[i] == default_fd) {
                    continue;
                }
                FD_SET(fd_array_[i], &rfds);
                max_fds = std::max(max_fds, fd_array_[i]);
            }
            int n = select(max_fds + 1, &rfds, nullptr, nullptr, nullptr);
            switch (n) {
                case 0:
                    std::cout << "timeout ..." << std::endl;
                    break;
                case -1:
                    std::cout << "select error, " << errno << "(" << strerror(errno) << ")" << std::endl;
                    break;
                default:
                    std::cout << "select success" << std::endl;
                    handlerEvent(rfds);
                    break;
            }
        }
    }
    void handlerEvent(const fd_set &rfds) {
        if (FD_ISSET(listen_sockfd_, &rfds)) {
            std::string ip;
            uint16_t port;
            int sockfd = Sock::Accept(listen_sockfd_, &ip, &port);
            if (sockfd == -1) {
                return;
            }
            int i = 0;
            for (; i < fd_num; ++i) {
                if (fd_array_[i] == default_fd) {
                    break;
                }
            }
            if (i == fd_num) {
                std::cout << "server fd is full, please wait" << std::endl;
                close(1);
            }
            fd_array_[i] = sockfd;
            printFdArray();
        }
    }
    void printFdArray() {
        std::cout << "fds: ";
        for (int i = 0; i < fd_num; ++i) {
            if (fd_array_[i] != default_fd) {
                std::cout << fd_array_[i] << " ";
            } else {
                break;
            }
        }
        std::cout << std::endl;
    }
    ~SelectServer () {
        delete []fd_array_;
    }
private:
    uint16_t port_;
    int listen_sockfd_;
    int *fd_array_;
};