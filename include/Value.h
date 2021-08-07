//
// Created by berci on 2021. 08. 04..
//

#pragma once

#include <string>
#include <utility>

class LoxValue {
public:
    virtual ~LoxValue() = default;
};

class LoxString : public LoxValue {
public:
    std::string value;

    explicit LoxString(std::string value_) : value{std::move(value_)} {}

    ~LoxString() override = default;
};

class LoxNumber : public LoxValue {
public:
    double value;

    explicit LoxNumber(double value_) : value{value_} {}

    ~LoxNumber() override = default;
};