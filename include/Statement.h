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

    using VariableStatementPtr = std::shared_ptr<VariableStatement>;

    class BlockStatement : public Statement {
    public:
        std::vector<StatementPtr> statements;
        explicit BlockStatement(std::vector<StatementPtr> statements_) : statements{std::move(statements_)} {}

        ~BlockStatement() override = default;
    };

    using BlockStatementPtr = std::shared_ptr<BlockStatement>;

    class IfStatement : public Statement {
    public:
        ExprPtr condition;
        BlockStatementPtr trueBranch;
        BlockStatementPtr falseBranch;

        IfStatement(ExprPtr condition_, BlockStatementPtr true_, BlockStatementPtr false_)
            : condition{std::move(condition_)}, trueBranch{std::move(true_)}, falseBranch{std::move(false_)} {}

        ~IfStatement() override = default;
    };

    using IfStatementPtr = std::shared_ptr<IfStatement>;

    class WhileStatement : public Statement {
    public:
        ExprPtr condition;
        BlockStatementPtr body;

        WhileStatement(ExprPtr condition_, BlockStatementPtr body_) : condition{std::move(condition_)}, body{std::move(body_)} {}

        ~WhileStatement() override = default;
    };

    using WhileStatementPtr = std::shared_ptr<WhileStatement>;
}