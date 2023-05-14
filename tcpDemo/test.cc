#include <iostream>
#include "Log.hpp"
#include "Thread.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"


int main()
{
    ThreadPool tp(10);
    tp.push(Task(ADD));
    tp.push(Task(SUB));

    return 0;
}