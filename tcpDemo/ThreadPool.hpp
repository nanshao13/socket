#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include "Task.hpp"
#include "Thread.hpp"
#include "Mutex.hpp"
#include "LockGuard.hpp"

class ThreadPool
{
public:
    ThreadPool(size_t threads)
    {
        for (;;)
        {
            workers_.push_back(new Thread(start_routine, this));
        }
    }
    static void *start_routine(void *args)
    {
        ThreadPool *threadpool = static_cast<ThreadPool *>(args);
        Task t;
        {
            LockGuard ld(threadpool->mutex_);
            while (threadpool->tasks_.empty())
            {
                pthread_cond_wait(&threadpool->cond_, &threadpool->mutex_.mutex_);
            }
            t = threadpool->pop();
        }
        t();
    }
    void push(const Task &in)
    {
        LockGuard ld(mutex_);
        tasks_.push(in);
        pthread_cond_signal(&cond_);
    }
    Task pop()
    {
        Task t;
        t = tasks_.front();
        tasks_.pop();
        return t;
    }
    ~ThreadPool() 
    {
        for (Thread *worker : workers_)
        {
            worker->join();
        }
    }

private:
    std::vector<Thread *> workers_;
    std::queue<Task> tasks_;
    Mutex mutex_;
    pthread_cond_t cond_;
};