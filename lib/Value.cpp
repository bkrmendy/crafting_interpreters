//
// Created by berci on 2021. 08. 07..
//

#include <Value.h>

namespace Lox {

    Value::~Value() = default;

    std::string String::toString() {
        return this->value;
    }

    std::string Number::toString() {
        return std::to_string(this->value);
    }

    std::string Boolean::toString() {
        if (this->value) {
            return "true";
        }
        return "false";
    }

    std::string Null::toString() {
        return "null";
    }
}
