//
// Created by berci on 2021. 07. 31..
//

#pragma once

#include <TokenType.h>
#include <Value.h>

#include <memory>
#include <string>
#include <utility>

namespace Lox {
    class Token {
    public:
        TokenType type_;
        std::string lexeme_;
        int line_;
        std::shared_ptr<Lox::Value> value_;


        Token(
                TokenType type,
                std::string lexeme,
                std::shared_ptr<Lox::Value> value,
                int line
        ) : type_{type}, lexeme_{std::move(lexeme)}, value_{std::move(value)}, line_{line} {}
    };
}
