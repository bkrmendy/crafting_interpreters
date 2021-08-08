//
// Created by berci on 2021. 08. 08..
//

#pragma once

#include <exception>
#include <string>
#include <utility>

#include <Token.h>
#include <TokenType.h>

namespace Lox {

    class ParseError : public std::exception {
        const char* message;
    public:
        ParseError(const Token& token, const std::string& message) {
            if (token.type_ == TokenType::END) {
                this->message = (std::to_string(token.line_) + " at end " + message).c_str();
            } else {
                this->message = (std::to_string(token.line_) + ": " + token.lexeme_ + ": " + message).c_str();
            }
        }

        const char * what() const noexcept override {
            return this->message;
        }

    };
}