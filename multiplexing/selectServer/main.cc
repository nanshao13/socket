#include <iostream>
#include <memory>
#include "selectServer.hpp"

void Usage(std::string proc) {
    std::cout << "usage:\n\t./" << proc << " port\n\n"; 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        Usage(argv[0]);
        exit(1);
    }
    std::unique_ptr<SelectServer> svr(new SelectServer(atoi(argv[1])));
    svr->init();
    svr->run();
    return 0;
}