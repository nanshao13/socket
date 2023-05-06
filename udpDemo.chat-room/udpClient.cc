#include <iostream>
#include <memory>
#include "udpClient.hpp"

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " server_ip" << " server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    std::unique_ptr<udpClient> ucit(new udpClient(serverip, serverport));
    ucit->init();
    ucit->start();
    return 0;
}