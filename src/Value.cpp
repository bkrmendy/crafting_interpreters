//
// Created by berci on 2021. 08. 07..
//

#include <Value.h>

namespace Lox {

    std::string String::toString() {
        return this->value;
    }

    std::string Number::toString() {
        return std::to_string(this->value);
    }
}
