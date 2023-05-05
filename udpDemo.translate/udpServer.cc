#include <iostream>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <memory>
#include <signal.h>
#include "udpServer.hpp"

const static std::string dictText = "./dict.txt";
static std::unordered_map<std::string, std::string> dict;

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

bool cutLine(const std::string &line, const std::string &separator, std::string *english, std::string *chinese)
{
    size_t pos = line.find(separator);
    if (pos == std::string::npos)
        return false;
    *english = line.substr(0, pos);
    *chinese = line.substr(pos + separator.size());
    return true;
}

static void dictInit()
{
    std::ifstream in(dictText, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "ifstream error" << errno << std::endl;
        exit(IFSTREAM_ERROR);
    }
    std::string line;
    std::string chinese;
    std::string english;
    while (getline(in, line))
    {
        if (cutLine(line, ":", &english, &chinese))
            dict.insert(make_pair(english, chinese));
    }
    in.close();
    std::cout << "dictInit success!" << std::endl;
}

void reload(int signo)
{
    (void)signo;
    dictInit();
}

// demo 英汉翻译
void translate(int sockfd, int16_t port, std::string ip, std::string message)
{
    std::string response;
    auto iter = dict.find(message);
    if (iter == dict.end())
        response = "Unknow";
    else
        response = iter->second;
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
    dictInit();
    signal(2, reload);
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<udpServer> usvr(new udpServer(translate, port));
    usvr->init();
    usvr->start();

    return 0;
}