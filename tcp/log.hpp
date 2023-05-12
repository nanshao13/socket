#pragma once

#include <iostream>
#include <string>

enum ERROR_LEVEL
{  
    DEBUG = 0,
    NORMAL,
    WARNING,
    ERROR,
    FATAL
};

void logMessage(int level, const std::string &message)
{
    std::cerr << message << std::endl;
}