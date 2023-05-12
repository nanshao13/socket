#include "tcpClient.hpp"
#include <memory>

void usage(std::string proc)
{
    std::cout << "usage:\n\t" << proc << " server_ip server_port\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    std::unique_ptr<tcpClient> ucit(new tcpClient(server_ip, server_port));
    ucit->start();
    return 0;
}