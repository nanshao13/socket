#include "tcpServer.hpp"
#include <memory>

void usage(std::string proc)
{
    std::cout << "usage:\n\t" << proc << " server_port\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }
    uint16_t server_port = atoi(argv[1]);
    std::unique_ptr<tcpServer> usvr(new tcpServer(server_port));
    usvr->start();

    return 0;
}