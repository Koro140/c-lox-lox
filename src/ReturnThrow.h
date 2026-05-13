#pragma once

#include <stdexcept>
#include <any>

class ReturnThrow : public std::runtime_error
{
private:
    /* data */
public:
    std::any value;
    ReturnThrow(std::any value) : runtime_error(""), value(value) {}
    ~ReturnThrow() {}
};