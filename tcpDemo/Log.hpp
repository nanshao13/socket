#pragma once

#include <iostream>
#include <string>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

#define LOG_FILE_DEBUG "log.debug"
#define LOG_FILE_NORMAL "log.normal"
#define LOG_FILE_WARNING "log.warning"
#define LOG_FILE_ERROR "log.error"
#define LOG_FILE_FATAL "log.fatal"

#define BUFFER_NUMBER 1024

const char *LevelToString(int level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case NORMAL:
        return "NORMAL";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case FATAL:
        return "FATAL";
    default:
        return nullptr;
    }
}

void OutputFile(std::string fileName, std::string message)
{
    int fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
    write(fd, message.c_str(), message.size());
    close(fd);
}

void LogMessage(int level, const char *format, ...)
{
    std::string log;
    char logPrefix[BUFFER_NUMBER];
    char logContent[BUFFER_NUMBER];
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    std::string time = ctime(&tv.tv_sec);
    time.resize(time.size() - 1);
    snprintf(logPrefix, sizeof logPrefix, "[%s][%s][pid: %d]: ", LevelToString(level), time.c_str(), getpid());
    va_list arg;
    va_start(arg, format);
    vsnprintf(logContent, sizeof logContent, format, arg);
    va_end(arg);
    log += logPrefix;
    log += logContent;
    log += ".\n";
    switch (level)
    {
    case DEBUG:
        OutputFile(LOG_FILE_DEBUG, log);
        break;
    case NORMAL:
        OutputFile(LOG_FILE_NORMAL, log);
        break;
    case WARNING:
        OutputFile(LOG_FILE_WARNING, log);
        break;
    case ERROR:
        OutputFile(LOG_FILE_ERROR, log);
        break;
    case FATAL:
        OutputFile(LOG_FILE_FATAL, log);
        break;
    default:
        break;
    }
}