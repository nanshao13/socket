#include "TcpClient.hpp"

void usage(std::string proc)
{
    std::cout << "usage:\n\t" << proc << " ip port\n";
}

int main(int argc, char *args[])
{
    if (argc != 3)
    {
        LogMessage(FATAL, "use error");
        usage(args[0]);
        exit(UES_ERROR);
    }
    std::string ip = args[1];
    uint16_t port = atoi(args[2]);
    TcpClient tc(ip, port);
    tc.run();
    return 0;
}
