#include "TcpServer.hpp"

void usage(std::string proc)
{
    std::cout << "usage:\n\t" << proc << " ip port\n";
}

int main(int argc, char *args[])
{
    if (argc != 2)
    {
        LogMessage(FATAL, "use error");
        usage(args[0]);
        exit(UES_ERROR);
    }
    uint16_t port = atoi(args[1]);
    TcpServer ts(port);
    ts.run();
    return 0;
}