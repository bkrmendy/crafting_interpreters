//
// Created by berci on 2021. 08. 08..
//

#pragma once

#include <Token.h>
#include <Expr.h>

#include <memory>
#include <utility>
#include <vector>

namespace Lox {

    class Parser {
        std::vector<Token> tokens_;
        int current_;
    public:

        explicit Parser(std::vector<Token> tokens) : tokens_{std::move(tokens)}, current_{0} {}

        ExprPtr parse();

        ExprPtr expression();

        ExprPtr equality();

        ExprPtr comparison();

        ExprPtr term();

        ExprPtr factor();

        ExprPtr unary();

        ExprPtr primary();

        bool match(const std::vector<TokenType>& types);

        bool check(const TokenType& type);

        bool is_at_end();

        Token advance();

        Token previous();

        void consume(TokenType type, const std::string& message);

        void synchronize();

    };
}
