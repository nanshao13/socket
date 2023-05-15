#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "Thread.hpp"
#include "Mutex.hpp"
#include "Condition.hpp"
#include "LockGuard.hpp"

template <class T>
class ThreadPool
{
private:
    ThreadPool(size_t threads)
    {
        for (size_t i = 0; i < threads; ++i)
        {
            workers_.push_back(new Thread(start_routine, this));
        }
    }
    static void *start_routine(void *args)
    {
        ThreadPool *tp = static_cast<ThreadPool *>(args);
        T task;
        {
            LockGuard lock(tp->mutex_);
            while (tp->tasks_.empty())
            {
                tp->condition_.wait(tp->mutex_);
            }
            task = tp->tasks_.front();
            tp->tasks_.pop();
        }
        task();
        return nullptr;
    }
    ~ThreadPool()
    {
        for (Thread *worker : workers_)
        {
            worker->join();
        }
    }

public:
    static ThreadPool<T> *getInstance(size_t threads)
    {
        if (instance == nullptr)
        {
            {
                LockGuard lock(singleton_mutex);
                if (instance == nullptr)
                {
                    instance = new ThreadPool(threads);
                }
            }
        }
        return instance;
    }
    void enqueue(const T &task)
    {
        {
            LockGuard lock(mutex_);
            tasks_.push(task);
        }
        condition_.notify_all();
    }

private:
    std::vector<Thread *> workers_;
    std::queue<T> tasks_;
    Mutex mutex_;
    Condition condition_;
    // 下面是为了实现单例（懒汉）模式
    static ThreadPool<T> *instance;
    static Mutex singleton_mutex;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::instance = nullptr;

template <class T>
Mutex ThreadPool<T>::singleton_mutex;
