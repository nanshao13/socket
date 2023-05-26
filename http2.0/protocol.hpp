#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const std::string sep = " ";
const std::string line_sep = "\r\n";
const std::string default_root = "./wwwroot";
const std::string html_home_page = "./wwwroot/home_page.html";
const std::string html_404 = "./wwwroot/404.html";

class Request
{
public:
    Request() {}
    bool Parse(const std::string &data)
    {
        size_t pos = data.find(line_sep);
        std::string status_ = data.substr(0, pos);
        std::stringstream ss(status_);
        ss >> method >> url >> version;
        // 访问的资源的路径
        if (url.size() == 1 && url[0] == '/')
            path = html_home_page;
        else
            path = default_root + url;
        // 访问的资源的后缀
        pos = url.rfind('.');
        if (pos == std::string::npos)
            suffix = ".html";
        else
            suffix = url.substr(pos);
        // 访问的资源的大小
        struct stat st;
        if (stat(path.c_str(), &st) == 0)
            size = st.st_size;
        else
        {
            // 404的大小
            stat(html_404.c_str(), &st);
            size = st.st_size;
        }
        return true;
    }
    ~Request() {}
    std::string method;
    std::string url;
    std::string version;
    std::string header;
    std::string blank;
    std::string body;
    std::string path;
    std::string suffix;
    int size;
};

class Response
{
public:
    Response()
    {
    }
    bool Serialize(std::string *output)
    {
        *output = "";
        *output += status;
        *output += header;
        *output += blank;
        *output += body;
        return true;
    }
    ~Response() {}
    std::string status;
    std::string header;
    std::string blank;
    std::string body;
};