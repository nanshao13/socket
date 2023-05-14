#pragma once

#include <iostream>
#include <pthread.h>
#include "Mutex.hpp"

class LockGuard
{
public:
    LockGuard(const Mutex &mutex);
    ~LockGuard();
private:
    Mutex mutex_;
};

inline LockGuard::LockGuard(const Mutex &mutex)
    : mutex_(mutex)
{
    mutex_.lock();
}

inline LockGuard::~LockGuard()
{
    mutex_.unlock();
}