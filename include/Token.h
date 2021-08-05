//
// Created by berci on 2021. 07. 31..
//

#pragma once

#include <TokenType.h>
#include <LoxValue.h>

#include <memory>
#include <string>
#include <utility>

class Token {
public:
    TokenType type_;
    std::string lexeme_;
    int line_;
    std::shared_ptr<LoxValue> value_;


    Token(
            TokenType type,
            std::string lexeme,
            std::shared_ptr<LoxValue> value,
            int line
    ) : type_{type}, lexeme_{std::move(lexeme)}, value_{std::move(value)}, line_{line} {}
};