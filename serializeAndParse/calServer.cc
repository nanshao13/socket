#include "CalServer.hpp"

void usage(std::string proc)
{
    std::cout << "usage:\n\t" << proc << " port\n";
}

enum
{
    OK = 0,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERROR
};

bool cal(const Request &req, Response *res)
{
    (*res).exit_code_ = OK;
    (*res).result_ = OK;
    switch (req.op_)
    {
    case '+':
        (*res).result_ = req.lhs_ + req.rhs_;
        break;
    case '-':
        (*res).result_ = req.lhs_ - req.rhs_;
        break;
    case '*':
        (*res).result_ = req.lhs_ * req.rhs_;
        break;
    case '/':
    {
        if (req.rhs_ == 0)
            (*res).exit_code_ = DIV_ZERO;
        else
            (*res).result_ = req.lhs_ / req.rhs_;
    }
    break;
    case '%':
    {
        if (req.rhs_ == 0)
            (*res).exit_code_ = MOD_ZERO;
        else
            (*res).result_ = req.lhs_ % req.rhs_;
    }
    break;
    default:
        (*res).exit_code_ = OP_ERROR;
        break;
    }
    return true;
}

int main(int argc, char *args[])
{
    if (argc != 2)
    {
        LogMessage(FATAL, "use error");
        usage(args[0]);
        exit(UES_ERROR);
    }
    uint16_t port = atoi(args[1]);
    TcpServer ts(port);
    ts.run(cal);
    return 0;
}