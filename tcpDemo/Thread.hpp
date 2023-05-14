#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>
#include "Log.hpp"

enum
{
    CREATE_THREAD_ERROR = 1
};

class Thread
{
public:
    Thread(std::function<void *(void *)> callback, void *args)
        : callback_(callback), args_(args)
    {
        if (pthread_create(&tid_, nullptr, start_routine, this) != 0)
        {
            LogMessage(FATAL, "create thread error");
            exit(CREATE_THREAD_ERROR);
        }
    }
    static void *start_routine(void *args)
    {
        Thread *thread = static_cast<Thread *>(args);
        return thread->callback_(thread->args_);
    }
    void join()
    {
        pthread_join(tid_, nullptr);
    }
    ~Thread() {}

private:
    pthread_t tid_;
    std::function<void *(void *)> callback_;
    void *args_;
};