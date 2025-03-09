#pragma once

#include <exception>
#include <string>

namespace gkr
{
namespace testing
{

struct text_exception : std::exception
{
    std::string text;

    text_exception(const char* str) : text(str) {}

    virtual const char* what() const noexcept override { return text.c_str(); }
};

}
}
