//
// Created by berci on 2021. 08. 29..
//

#pragma once

#include <Expr.h>

#include <memory>
#include <utility>
#include <vector>

namespace Lox {
    class Statement {
    public:
        virtual ~Statement() = 0;
    };

    using StatementPtr = std::shared_ptr<Statement>;

    class ExpressionStatement : public Statement {
    public:
        ExprPtr expression;

        explicit ExpressionStatement(ExprPtr expr) : expression{std::move(expr)} {}

        ~ExpressionStatement() override = default;
    };

    using ExprStatementPtr = std::shared_ptr<ExpressionStatement>;

    class PrintStatement : public Statement {
    public:
        ExprPtr expression;

        explicit PrintStatement(ExprPtr expr) : expression{std::move(expr)} {}
        ~PrintStatement() override = default;
    };

    using PrintStatementPtr = std::shared_ptr<PrintStatement>;

    class VariableStatement : public Statement {
    public:
        Token name;
        ExprPtr initializer;

        VariableStatement(Token name_, ExprPtr initializer_) : name{std::move(name_)}, initializer{std::move(initializer_)} {}

        ~VariableStatement() override = default;
    };

    class BlockStatement : public Statement {
    public:
        std::vector<StatementPtr> statements;
        explicit BlockStatement(std::vector<StatementPtr> statements_) : statements{std::move(statements_)} {}

        ~BlockStatement() override = default;
    };
}