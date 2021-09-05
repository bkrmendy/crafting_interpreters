//
// Created by berci on 2021. 08. 08..
//

#include <Parser.h>
#include <Value.h>
#include <Expr.h>
#include <ParseError.h>
#include <utils.h>

#include <iostream>

namespace Lox {

    ExprPtr Parser::expression() {
        return assignment();
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
        return this->tokens_.at(this->current_).type_ == TokenType::END;
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

        if (match({ TokenType::IDENTIFIER })) {
            return std::make_shared<Var>(previous());
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

    Token Parser::consume(TokenType type, const std::string &message) {
        if (check(type)) {
            return advance();
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

    std::vector<StatementPtr> Parser::parse() {
        std::vector<StatementPtr> statements{};
        try {
            while (not is_at_end()) {
                auto stmt = declaration();
                statements.push_back(stmt);
            }
            return statements;
        } catch (ParseError& p) {
            std::cout << p.what() << std::endl;
            return std::vector<StatementPtr>{};
        }
    }

    StatementPtr Parser::statement() {
        if (match({ TokenType::PRINT })) {
            return printStatement();
        }

        if (match({ TokenType::LEFT_BRACE })) {
            return block();
        }

        return expressionStatement();
    }

    StatementPtr Parser::expressionStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_shared<ExpressionStatement>(value);
    }

    StatementPtr Parser::printStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_shared<PrintStatement>(value);
    }

    StatementPtr Parser::declaration() {
        try {
            if (match({ TokenType::VAR })) {
                return variableDeclaration();
            }
            return statement();
        } catch (ParseError&) {
            synchronize();
            return nullptr;
        }
    }

    StatementPtr Parser::variableDeclaration() {
        Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
        ExprPtr initializer = nullptr;
        if (match({ TokenType::EQUAL })) {
            initializer = expression();
        }
        consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
        return std::make_shared<VariableStatement>(name, initializer);
    }

    ExprPtr Parser::assignment() {
        ExprPtr expr = equality();

        if (match({ TokenType::EQUAL })) {
            Token equals = previous();
            ExprPtr value = assignment();

            if (auto var = std::dynamic_pointer_cast<Var>(expr)) {
                Token name = var->name;
                return std::make_shared<Assignment>(name, expr);
            }

            error(equals.line_, "Invalid assignment target.");
        }

        return expr;
    }

    StatementPtr Parser::block() {
        std::vector<StatementPtr> statements{};

        while (not check({ TokenType::RIGHT_BRACE }) and not is_at_end()) {
            statements.push_back(declaration());
        }

        consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");

        return std::make_shared<BlockStatement>(statements);
    }
}
