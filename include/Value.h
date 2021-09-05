//
// Created by berci on 2021. 08. 04..
//

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace Lox {
    class Value {
    public:
        virtual std::string toString() = 0;
        virtual ~Value() = 0;
    };

    class String : public Value {
    public:
        std::string value;

        explicit String(std::string value_) : value{std::move(value_)} {}

        std::string toString() final;

        ~String() override = default;
    };

    class Number : public Value {
    public:
        double value;

        explicit Number(double value_) : value{value_} {}

        std::string toString() final;

        ~Number() override = default;
    };

    class Boolean : public Value {
    public:
        bool value;

        explicit Boolean(bool value_) : value{value_} {}

        std::string toString() final;

        ~Boolean() override = default;
    };

    class Null : public Value {
    public:
        Null() = default;

        std::string toString() final;

        ~Null() override = default;
    };

    // forward decl
    class BlockStatement;

    class Function : public Value {
    public:
        std::vector<std::string> params;
        std::shared_ptr<BlockStatement> body;

        Function(std::vector<std::string> params_, std::shared_ptr<BlockStatement> body_)
            : params{std::move(params_)}, body{std::move(body_)} {}

        std::string toString() override;

        ~Function() override = default;
    };

    using FunctionPtr = std::shared_ptr<Function>;
}

