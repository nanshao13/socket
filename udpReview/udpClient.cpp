#include <iostream>
#include <memory>
#include "udpClient.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " serverip" << " serverport" << std::endl; 
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]); 
    std::unique_ptr<udpClient> ucit(new udpClient(port, ip));
    ucit->init();
    ucit->start();
    return 0;
}
