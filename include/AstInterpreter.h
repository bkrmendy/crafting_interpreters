//
// Created by berci on 2021. 08. 09..
//

#include <Visitor.h>
#include <Expr.h>
#include <Value.h>

#include <memory>

namespace Lox {
    std::shared_ptr<Value> interpret(const ExprPtr& expression);
}