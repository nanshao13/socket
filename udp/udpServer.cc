#include <iostream>
#include <memory>
#include "udpServer.hpp"

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<udpServer> usvr(new udpServer(port));
    usvr->init();
    usvr->start();

    return 0;
}