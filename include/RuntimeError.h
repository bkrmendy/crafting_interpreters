//
// Created by berci on 2021. 08. 09..
//

#pragma once

#include <string>
#include <exception>

namespace Lox {
    class RuntimeError : public std::exception {
        const char* message_;

    public:
        explicit RuntimeError(const std::string& message) : message_{message.c_str()} {}

        const char * what() const noexcept override;
    };
}