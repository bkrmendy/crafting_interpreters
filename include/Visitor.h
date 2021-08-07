//
// Created by berci on 2021. 08. 07..
//

#pragma once

#include <Expr.h>

#include <functional>

namespace Lox {

    template <typename T>
    class Visitor {
    public:
        using ExprPtr = std::shared_ptr<Expr>;
        using VisitorFn = std::function<T(ExprPtr)>;

    private:
        VisitorFn visitor_;

    public:

        explicit Visitor(VisitorFn visitorFn) : visitor_{std::move(visitorFn)} {}

        T run(ExprPtr expr) {
            return visitor_(expr);
        }

        ~Visitor() = default;
    };

}
