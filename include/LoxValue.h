//
// Created by berci on 2021. 08. 04..
//

#pragma once

#include <string>
#include <utility>

class LoxValue {
public:
    ~LoxValue() = default;
};

class LoxString : public LoxValue {
public:
    std::string value;

    explicit LoxString(std::string value_) : value{std::move(value_)} {}

    ~LoxString() = default;
};

class LoxNumber : public LoxValue {
public:
    double value;

    explicit LoxNumber(double value_) : value{value_} {}

    ~LoxNumber() = default;
};