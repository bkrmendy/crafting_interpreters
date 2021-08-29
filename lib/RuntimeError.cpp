//
// Created by berci on 2021. 08. 28..
//

#include "RuntimeError.h"

namespace Lox {
    const char* RuntimeError::what() const noexcept {
        return this->message_;
    }
}
