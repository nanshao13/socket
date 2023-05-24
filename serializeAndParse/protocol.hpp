#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <jsoncpp/json/json.h>

#define SEP " "
#define SEP_LEN strlen(SEP)
#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

// "length message\r\n"
// 35 {"left":1,"operator":43,"right":2}\r\n

std::string enLength(const std::string &content)
{
    std::string package = std::to_string(content.size());
    package += SEP;
    package += content;
    package += LINE_SEP;
    return package;
}
std::string deLength(const std::string &package)
{
    size_t pos = package.find(SEP);
    std::string content_length_str = package.substr(0, pos);
    int content_length = atoi(content_length_str.c_str());
    std::string content = package.substr(pos + SEP_LEN, content_length);
    return content;
}

bool recvPackage(int sock, std::string &inbuffer, std::string *package)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            inbuffer += buffer;
            auto pos = inbuffer.find(LINE_SEP);
            if (pos == std::string::npos)
            {
                continue;
            }
            *package = "";
            *package = inbuffer.substr(0, pos + LINE_SEP_LEN);
            inbuffer.erase(0, pos + LINE_SEP_LEN);
            break;
        }
        else 
        {
            return false;
        }
    }
    return true;
}

class Request
{
public:
    Request()
    {
    }
    Request(int lhs, int rhs, char op)
        : lhs_(lhs), rhs_(rhs), op_(op)
    {
    }
    bool Serialize(std::string *out)
    {
        (*out).clear();
        Json::Value root;
        root["left"] = lhs_;
        root["right"] = rhs_;
        root["operator"] = op_;
        Json::FastWriter writer;
        *out = writer.write(root);
        return true;
    }
    bool Parse(const std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);
        lhs_ = root["left"].asInt();
        rhs_ = root["right"].asInt();
        op_ = root["operator"].asInt();
        return true;
    }

public:
    int lhs_;
    int rhs_;
    char op_;
};

class Response
{
public:
    Response()
    {
    }
    Response(int exit_code, int result)
        : exit_code_(exit_code), result_(result)
    {
    }
    bool Serialize(std::string *out)
    {
        (*out).clear();
        Json::Value root;
        root["exit_code"] = exit_code_;
        root["result"] = result_;
        Json::FastWriter writer;
        *out = writer.write(root);
        return true;
    }
    bool Parse(const std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);
        exit_code_ = root["exit_code"].asInt();
        result_ = root["result"].asInt();
        return true;
    }

public:
    int exit_code_;
    int result_;
};