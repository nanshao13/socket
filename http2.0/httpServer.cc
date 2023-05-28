#include <iostream>
#include <string>
#include <cstdlib>
#include "httpServer.hpp"

void Usage(std::string process)
{
    std::cout << "Usage:\n\t" << process << " port" << std::endl;
}

std::string suffixToRescource(const std::string &suffix)
{
    if (suffix == ".html") return "text/html";
    if (suffix == ".jpg") return "application/x-jpg;image/jpeg";
    if (suffix == ".png") return "application/x-png;image/png";
    return "text/html";
}

void Conversion(const Request &request, Response *response)
{
    response->status = "HTTP/1.1 200 OK\r\n";
    // response->status = "HTTP/1.1 307 Temporary Redirect\r\n";
    response->header = "Content-Type: " + suffixToRescource(request.suffix) + "\r\n";
    //if (request.size > 0) //这里填充Content-Length是有问题的
    //    response->header += "Content-Length: " + std::to_string(request.size) + "\r\n";
    // response->header += "Location: https://www.qq.com/\r\n";
    response->blank = "\r\n";
    if (!Util::readFile(request.path, &response->body))
    {
        Util::readFile(html_404, &response->body);
    }
    if (response->body.size() > 0)
        response->header += "Content-Length: " + std::to_string(response->body.size()) + "\r\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    HttpServer svr(port, Conversion);
    svr.Run();
    return 0;
}