//
// Created by berci on 2021. 07. 31..
//

#include <fmt/core.h>

void report(int line, const std::string& where, const std::string& message) {
    fmt::print("[line {}]: Error {}: {}", line, where, message);
}

void error(int line, const std::string& message) {
    report(line, "", message);
}
