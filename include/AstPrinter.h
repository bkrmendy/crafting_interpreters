//
// Created by berci on 2021. 08. 07..
//

#pragma once

#include <Expr.h>

#include <string>
#include <memory>

namespace Lox {
    std::string AstPrinter(const std::shared_ptr<Expr>& expr);
}
