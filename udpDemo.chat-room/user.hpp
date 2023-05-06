#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class user
{
public:
    user(const std::string &ip, const uint16_t &port, const std::string nickname)
        : ip_(ip), port_(port), nickname_(nickname)
    {
    }
    ~user()
    {
    }

    std::string ip_;
    uint16_t port_;
    std::string nickname_;
};

class onlineUser
{
public:
    void addUser(const std::string &ip, const uint16_t &port, const std::string &nickname)
    {
        std::string id = ip + std::to_string(port);
        users.insert(make_pair(id, user(ip, port, nickname)));
    }
    void delUser(const std::string &ip, const uint16_t &port)
    {
        std::string id = ip + std::to_string(port);
        users.erase(id);
    }
    bool isOnline(const std::string &ip, const uint16_t &port)
    {
        std::string id = ip + std::to_string(port);
        if (users.find(id) == users.end())
        {
            return false;
        }
        return true;
    }
    void broadCastMessage(const int &sockfd, const std::string ip, const uint16_t &port, const std::string &message)
    {
        std::string nickname;
        for (auto &u : users)
        {
            if (u.second.ip_ == ip && u.second.port_ == port)
            {
                nickname = u.second.nickname_;
            }
        }
        for (auto &u : users)
        {
            std::string response = nickname + "# " + message;
            struct sockaddr_in dest_addr;
            bzero(&dest_addr, sizeof dest_addr);
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_addr.s_addr = inet_addr(u.second.ip_.c_str());
            dest_addr.sin_port = htons(u.second.port_);
            sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&dest_addr, sizeof dest_addr);
        }
    }

private:
    std::unordered_map<std::string, user> users;
};