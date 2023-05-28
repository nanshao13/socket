#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Util
{
public:
    static bool readFile(const std::string &resource, std::string *output)
    {
        std::ifstream in(resource, std::ios::binary);
        if (!in.is_open()) return false;
        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);
        std::vector<char> buffer(fileSize);
        in.read(buffer.data(), fileSize);
        (*output) = buffer.data();
        in.close();
        return true;
    }
};