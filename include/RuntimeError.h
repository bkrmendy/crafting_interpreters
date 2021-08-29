//
// Created by berci on 2021. 08. 09..
//

#pragma once

#include <exception>

namespace Lox {
    class RuntimeError : public std::exception {
        const char* message_;

    public:
        RuntimeError(const char* message) : message_{message} {}

        const char * what() const noexcept override;
    };
}