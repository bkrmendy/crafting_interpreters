//
// Created by berci on 2021. 08. 09..
//

#include <Expr.h>
#include <Value.h>
#include <Statement.h>
#include <Environment.h>

#include <memory>
#include <functional>
#include <string>

namespace Lox {
    using Writer = std::function<void(const std::string&)>;

    std::shared_ptr<Value> interpret(const std::shared_ptr<Environment>& env, const ExprPtr& expression, const Writer& writer);
    std::shared_ptr<Value> interpret(const std::shared_ptr<Environment>& env,const StatementPtr& statement, const Writer& writer);
}