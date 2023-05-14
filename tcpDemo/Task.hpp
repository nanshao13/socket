#pragma

#include <iostream>

void ADD()
{
    while (true)
    {
        std::cout << "add" << std::endl;
    }
}

void SUB()
{
    while (true)
    {
        std::cout << "sub" << std::endl;
    }
}

class Task
{
public:
    Task()
    {
    }
    Task(std::function<void()> func)
        : _callback(func)
    {
    }
    void operator()()
    {
        _callback();
    }

private:
    std::function<void()> _callback;
};
