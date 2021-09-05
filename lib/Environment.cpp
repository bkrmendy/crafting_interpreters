//
// Created by berci on 2021. 09. 05..
//

#include <Environment.h>
#include <RuntimeError.h>

#include <utility>

namespace Lox {

    void Environment::define(const std::string& name, std::shared_ptr<Value> value) {
        this->env_[name] = std::move(value);
    }

    std::shared_ptr<Value> Environment::get(const std::string &name) const {
        if (this->env_.contains(name)) {
            return this->env_.at(name);
        }

        if (this->enclosing_ != nullptr) {
            return this->enclosing_->get(name);
        }

        throw RuntimeError("Undefined variable: " + name);
    }

    void Environment::assign(const std::string &name, std::shared_ptr<Value> value) {
        if (this->env_.contains(name)) {
            this->env_[name] = std::move(value);
        }

        if (this->enclosing_ != nullptr) {
            this->enclosing_->assign(name, value);
        }
        throw RuntimeError("Undefined variable: " + name);
    }
}
