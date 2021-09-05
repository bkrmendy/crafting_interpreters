//
// Created by berci on 2021. 09. 05..
//

#pragma once

#include <Value.h>

#include <map>
#include <string>
#include <memory>

namespace Lox {

    class Environment {
        std::shared_ptr<Environment> enclosing_{};
        std::map<std::string, std::shared_ptr<Value>> env_{};
    public:
        Environment() = default;
        explicit Environment(std::shared_ptr<Environment> enclosing) : enclosing_{std::move(enclosing)} {}
        void define(const std::string& name, std::shared_ptr<Value> value);
        void assign(const std::string& name, std::shared_ptr<Value> value);
        std::shared_ptr<Value> get(const std::string& name) const;
    };
}
