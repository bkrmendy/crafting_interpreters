//
// Created by berci on 2021. 08. 28..
//

#include <TypeCast.h>
#include <RuntimeError.h>

namespace Lox {
    std::shared_ptr<Number> as_number(const std::shared_ptr<Value>& value) {
        if (auto n = std::dynamic_pointer_cast<Number>(value)) {
            return n;
        }
        throw RuntimeError("Attempted to cast to number");
    }

    std::shared_ptr<Boolean> as_boolean(const std::shared_ptr<Value>& value) {
        if (auto b = std::dynamic_pointer_cast<Boolean>(value)) {
            return b;
        }
        throw RuntimeError("Attempted to cast to boolean");

    }

    bool is_truthy(const std::shared_ptr<Value>& value) {
        if (auto number = std::dynamic_pointer_cast<Number>(value)) {
            return number->value > 0;
        }

        if (auto boolean = std::dynamic_pointer_cast<Boolean>(value)) {
            return boolean->value;
        }

        if (std::dynamic_pointer_cast<Null>(value)) {
            return true;
        }
    }

    bool is_equal(const std::shared_ptr<Value>& left, const std::shared_ptr<Value>& right) {
        if (auto l = std::dynamic_pointer_cast<String>(left)) {
            if (auto r = std::dynamic_pointer_cast<String>(right)) {
                return l->value == r->value;
            }
        }

        if (auto l = std::dynamic_pointer_cast<Number>(left)) {
            if (auto r = std::dynamic_pointer_cast<Number>(right)) {
                return l->value == r->value;
            }
        }

        if (auto l = std::dynamic_pointer_cast<Boolean>(left)) {
            if (auto r = std::dynamic_pointer_cast<Boolean>(right)) {
                return l->value == r->value;
            }
        }

        if (std::dynamic_pointer_cast<Null>(left)) {
            if (std::dynamic_pointer_cast<Null>(right)) {
                return true;
            }
        }

        return false;
    }
}