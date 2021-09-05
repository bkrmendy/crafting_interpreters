//
// Created by berci on 2021. 07. 31..
//

#pragma once

#include <string>
#include <vector>

void report(int line, const std::string& where, const std::string& message);
void error(int line, const std::string& message);