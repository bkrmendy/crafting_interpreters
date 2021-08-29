//
// Created by berci on 2021. 08. 08..
//

#include <Parser.h>
#include <Value.h>
#include <Expr.h>
#include <ParseError.h>

#include <iostream>

namespace Lox {

    ExprPtr Parser::expression() {
        if (auto expr = this->equality()) {
            return expr;
        }
        if (auto expr = this->primary()) {
            return expr;
        }

        return nullptr;
    }

    ExprPtr Parser::equality() {
        ExprPtr expr = this->comparison();

        while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
            Token op = previous();
            ExprPtr right = comparison();
            expr = std::make_shared<Binary>(expr, right, op);
        }

        return expr;

    }

    ExprPtr Parser::comparison() {
        ExprPtr expr = term();

        while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
            Token op = previous();
            ExprPtr right = term();
            expr = std::make_shared<Binary>(expr, right, op);
        }

        return expr;
    }

    Token Parser::previous() {
        return tokens_.at(current_ - 1);
    }

    bool Parser::match(const std::vector<TokenType> &types) {

        for(TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    bool Parser::check(const TokenType &type) {
        if (is_at_end()) {
            return false;
        }

        return tokens_.at(current_).type_ == type;
    }

    bool Parser::is_at_end() {
        return current_ >= tokens_.size();
    }

    Token Parser::advance() {
        if (not is_at_end()) {
            current_ = current_ + 1;
        }
        return previous();
    }

    ExprPtr Parser::term() {
        ExprPtr expr = factor();

        while (match({ TokenType::MINUS, TokenType::PLUS })) {
            Token op = previous();
            ExprPtr right = factor();
            expr = std::make_shared<Binary>(expr, right, op);
        }

        return expr;
    }

    ExprPtr Parser::factor() {
        ExprPtr expr = unary();

        while (match({ TokenType::SLASH, TokenType::STAR })) {
            Token op = previous();
            ExprPtr right = unary();
            expr = std::make_shared<Binary>(expr, right, op);
        }

        return expr;
    }

    ExprPtr Parser::primary() {
        if (match({ TokenType::FALSE })) {
            return std::make_shared<Literal>(std::make_shared<Boolean>(false));
        }

        if (match({ TokenType::TRUE })) {
            return std::make_shared<Literal>(std::make_shared<Boolean>(true));
        }

        if (match({ TokenType::NIL })) {
            return std::make_shared<Literal>(std::make_shared<Null>());
        }

        if (match({ TokenType::NUMBER, TokenType::STRING })) {
            return std::make_shared<Literal>(previous().value_);
        }

        if (match({ TokenType::LEFT_PAREN })) {
            ExprPtr expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
            return std::make_shared<Grouping>(expr);
        }

        Token last = tokens_.at(current_);
        throw ParseError{last, "Expected expression."};
    }

    ExprPtr Parser::unary() {
        if (match({ TokenType::BANG, TokenType::MINUS })) {
            Token op = previous();
            ExprPtr right = unary();
            return std::make_shared<Unary>(op, right);
        }

        return primary();
    }

    void Parser::consume(TokenType type, const std::string &message) {
        if (check(type)) {
            advance();
            return;
        }

        Token last = tokens_.at(current_);
        throw ParseError{last, message};
    }

    void Parser::synchronize() {
        advance();

        while (not is_at_end()) {
            if (previous().type_ == TokenType::SEMICOLON) {
                return;
            }

            Token last = tokens_.at(current_);
            switch (last.type_) {
                case TokenType::CLASS:
                case TokenType::FUN:
                case TokenType::VAR:
                case TokenType::FOR:
                case TokenType::WHILE:
                case TokenType::PRINT:
                case TokenType::RETURN:
                    return;
                default: advance();
            }
        }
    }

    ExprPtr Parser::parse() {
        try {
            return expression();
        } catch (ParseError& p) {
            std::cout << p.what() << std::endl;
            return nullptr;
        }
    }
}
