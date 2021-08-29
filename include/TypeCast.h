//
// Created by berci on 2021. 08. 09..
//

#pragma once

#include <Value.h>

#include <memory>

namespace Lox {
    std::shared_ptr<Number> as_number(const std::shared_ptr<Value>& value);
    std::shared_ptr<Boolean> as_boolean(const std::shared_ptr<Value>& value);

    bool is_truthy(const std::shared_ptr<Value>& value);
    bool is_equal(const std::shared_ptr<Value>& left, const std::shared_ptr<Value>& right);
}