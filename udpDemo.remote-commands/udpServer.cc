#include <iostream>
#include <cstdio>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <memory>
#include <signal.h>
#include "udpServer.hpp"

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

// demo 远程命令
void remoteCommands(int sockfd, int16_t port, std::string ip, std::string command)
{
    std::string response;
    FILE *fp = popen(command.c_str(), "r");
    if (fp == nullptr)
    {
        std::cerr << "popen error" << std::endl;
        exit(POPEN_ERROR);
    }
    char line[1024];
    while (fgets(line, sizeof line, fp))
    {
        response += line;
    }
    pclose(fp);
    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof dest_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&dest_addr, sizeof dest_addr);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<udpServer> usvr(new udpServer(remoteCommands, port));
    usvr->init();
    usvr->start();

    return 0;
}