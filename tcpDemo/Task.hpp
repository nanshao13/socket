#pragma once

#include <iostream>
#include <unistd.h>
#include <functional>

void serviceIO(int sockfd)
{
    for (;;)
    {
        char buf[1024];
        ssize_t n = read(sockfd, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = 0;
        }
        else if (n == 0)
        {
            LogMessage(NORMAL, "client exit");
            break;
        }
        std::cout << "server read# " << buf << std::endl;
        std::string response = buf;
        response += "[server echo]";
        write(sockfd, response.c_str(), response.size());
    }
}

class Task
{
public:
    Task()
    {
    }
    Task(const std::function<void(int)> &callback, int sockfd)
        : callback_(callback), sockfd_(sockfd)
    {
    }
    void operator()()
    {
        callback_(sockfd_);
    }

private:
    std::function<void(int)> callback_;
    int sockfd_;
};
