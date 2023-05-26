#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Util
{
public:
    static bool readFile(const std::string &resource, char *output, int size)
    {
        std::ifstream in(resource, std::ios::binary);
        if (!in.is_open()) return false;
        in.read(output, size);
        in.close();
        return true;
    }
};