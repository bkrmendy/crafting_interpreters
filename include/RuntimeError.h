//
// Created by berci on 2021. 08. 09..
//

#pragma once

#include <string>
#include <exception>
#include <utility>

namespace Lox {
    class RuntimeError : public std::exception {
        std::string message_;

    public:
        explicit RuntimeError(std::string message) : message_{std::move(message)} {}

        const char * what() const noexcept override;
    };
}