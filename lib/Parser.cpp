//
// Created by berci on 2021. 08. 08..
//

#include <Parser.h>
#include <Value.h>
#include <Expr.h>
#include <ParseError.h>
#include <utils.h>

#include <vector>
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

        while (match({ TokenType::SLASH, TokenType::STAR, TokenType::MODULO, TokenType::AND, TokenType::OR })) {
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
            auto var = std::make_shared<Var>(previous());
            if (!match({ TokenType::LEFT_PAREN })) {
                return var;
            }

            std::vector<ExprPtr> arguments{};

            while (not check(TokenType::RIGHT_PAREN) and not is_at_end()) {
                auto arg = expression();
                arguments.push_back(arg);
                if (not check(TokenType::RIGHT_PAREN)) {
                    consume(TokenType::COMMA, "Expected `,`");
                }
            }

            consume(TokenType::RIGHT_PAREN, "Expected `)`");

            return std::make_shared<FunctionCall>(var->name.lexeme_, arguments);
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

        if (match({ TokenType :: IF })) {
            return ifStatement();
        }

        if (match({ TokenType::WHILE })) {
            return whileStatement();
        }

        if (match({ TokenType::FOR })) {
            return forStatement();
        }

        if (match({ TokenType::FUN })) {
            return functionDeclaration();
        }

        return expressionStatement();
    }

    ExprStatementPtr Parser::expressionStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_shared<ExpressionStatement>(value);
    }

    PrintStatementPtr Parser::printStatement() {
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

    VariableStatementPtr Parser::variableDeclaration() {
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
                return std::make_shared<Assignment>(name, value);
            }

            error(equals.line_, "Invalid assignment target.");
        }

        return expr;
    }

    BlockStatementPtr Parser::block() {
        std::vector<StatementPtr> statements{};

        while (not check({ TokenType::RIGHT_BRACE }) and not is_at_end()) {
            auto decl = declaration();
            statements.push_back(decl);
        }

        consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");

        return std::make_shared<BlockStatement>(statements);
    }

    IfStatementPtr Parser::ifStatement() {
        consume(TokenType::LEFT_PAREN, "Expected `(`");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expected `)`");
        consume(TokenType::LEFT_BRACE, "Expected `{`");
        auto trueBranch = block();

        auto falseBranch = std::shared_ptr<BlockStatement>(nullptr);
        if (match({ TokenType::ELSE })) {
            consume(TokenType::LEFT_BRACE, "Expected `{`");
            falseBranch = block();
        }
        return std::make_shared<IfStatement>(condition, trueBranch, falseBranch);
    }

    WhileStatementPtr Parser::whileStatement() {
        consume(TokenType::LEFT_PAREN, "Expected `(`");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expected `)`");
        consume(TokenType::LEFT_BRACE, "Expected `{`");
        auto body = block();

        return std::make_shared<WhileStatement>(condition, body);
    }

    BlockStatementPtr Parser::forStatement() {
        consume(TokenType::LEFT_PAREN, "Expected `(`");
        consume(TokenType::VAR, "Expected `var`");
        auto initialization = variableDeclaration();
        auto loopCondition = expression();
        consume(TokenType::SEMICOLON, "Expected `;`");
        auto increment = assignment();
        consume(TokenType::RIGHT_PAREN, "Expected `)`");
        consume(TokenType::LEFT_BRACE, "Expected `{");
        auto body = block();

        std::vector<StatementPtr> bodyWithIncrement = {
           body,
           std::make_shared<ExpressionStatement>(increment)
       };

        std::vector<StatementPtr> desugaredFor = {
            initialization,
            std::make_shared<WhileStatement>(loopCondition, std::make_shared<BlockStatement>(bodyWithIncrement))
        };

        return std::make_shared<BlockStatement>(desugaredFor);
    }

    FunctionDeclarationPtr Parser::functionDeclaration() {
        auto name = consume(TokenType::IDENTIFIER, "Expected an identifier").lexeme_;
        consume(TokenType::LEFT_PAREN, "Expected `(`");

        std::vector<std::string> params{};

        while (not check(TokenType::RIGHT_PAREN) and not is_at_end()) {
            auto param = consume(TokenType::IDENTIFIER, "Expected an identifier");
            params.push_back(param.lexeme_);
            if (not check(TokenType::RIGHT_PAREN)) {
                consume(TokenType::COMMA, "Expected `,`");
            }
        }
        consume(TokenType::RIGHT_PAREN, "Expected `)");
        consume(TokenType::LEFT_BRACE, "Expected `{");

        auto body = block();

        return std::make_shared<FunctionDeclaration>(name, params, body);
    }
}
