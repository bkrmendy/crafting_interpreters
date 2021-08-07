//
// Created by berci on 2021. 08. 07..
//

#pragma once

#include <Token.h>

#include <memory>
#include <utility>

namespace Lox {

    class Expr {
    public:
        virtual ~Expr() = 0;
    };

    using ExprPtr = std::shared_ptr<Expr>;

    class Binary : public Expr {
    public:
        std::shared_ptr<Expr> left;
        std::shared_ptr<Expr> right;
        std::shared_ptr<Token> op;

        Binary(ExprPtr left_, ExprPtr right_, std::shared_ptr<Token> op_)
            : left{std::move(left_)}, right{std::move(right_)}, op{std::move(op_)} {}

        ~Binary() override = default;
    };

    using BinaryPtr = std::shared_ptr<Binary>;

    class Grouping : public Expr {
    public:
        std::shared_ptr<Expr> expr;

        explicit Grouping(std::shared_ptr<Expr> expr_) : expr{std::move(expr_)} {}

        ~Grouping() override = default;
    };

    using GroupingPtr = std::shared_ptr<Grouping>;

    class Literal : public Expr {
    public:
        std::shared_ptr<Value> value;

        explicit Literal(std::shared_ptr<Value> value_) : value{std::move(value_)} {}

        ~Literal() override = default;

    };

    using LiteralPtr = std::shared_ptr<Literal>;

    class Unary : public Expr {
    public:
        std::shared_ptr<Token> op;
        std::shared_ptr<Expr> operand;

        Unary(std::shared_ptr<Token> op_, std::shared_ptr<Expr> operand_)
            : op{std::move(op_)}, operand{std::move(operand_)} {}

        ~Unary() override = default;
    };

    using UnaryPtr = std::shared_ptr<Unary>;

}




