#include <iostream>
#include <cstdio>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <memory>
#include <signal.h>
#include "udpServer.hpp"
#include "user.hpp"

onlineUser ou;

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

// demo 远程命令
void chatRoom(int sockfd, int16_t port, std::string ip, std::string message)
{
    std::string key = message.substr(0, message.find(" "));
    if (key == "online")
    {
        std::string nickname = message.substr(message.find(" ") + 1);
        ou.addUser(ip, port, nickname);
    }
    if (key == "offline")
        ou.delUser(ip, port);
    if (ou.isOnline(ip, port))
        ou.broadCastMessage(sockfd, ip, port, message);
    else
    {
        std::string response = "Please input \"online nickname\"";
        struct sockaddr_in dest_addr;
        bzero(&dest_addr, sizeof dest_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&dest_addr, sizeof dest_addr);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<udpServer> usvr(new udpServer(chatRoom, port));
    usvr->init();
    usvr->start();

    return 0;
}