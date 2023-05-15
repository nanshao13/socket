#pragma once

#include <iostream>
#include <pthread.h>
#include "Mutex.hpp"

class Condition
{
public:
    pthread_cond_t condition_;
    Condition()
    {
        pthread_cond_init(&condition_, nullptr);
    }
    void wait(Mutex &mutex)
    {
        pthread_cond_wait(&condition_, &mutex.mutex_);
    }
    void notify_one()
    {
        pthread_cond_signal(&condition_);
    }
    void notify_all()
    {
        pthread_cond_broadcast(&condition_);
    }
    ~Condition()
    {
        pthread_cond_destroy(&condition_);
    }
};