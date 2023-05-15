#pragma once

#include <iostream>
#include <pthread.h>
#include "Mutex.hpp"

class LockGuard
{
public:
    LockGuard(Mutex &mutex)
        : mutex_(&mutex)
    {
        mutex_->lock();
    }
    ~LockGuard()
    {
        mutex_->unlock();
    }

private:
    Mutex *mutex_;
};
