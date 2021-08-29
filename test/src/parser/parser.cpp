//
// Created by berci on 2021. 08. 28..
//

#include "gtest/gtest.h"
#include "ApprovalTests.hpp"

#include <Scanner.h>
#include <Parser.h>
#include <AstInterpreter.h>

#include <string>

void runSource(const std::string& source) {
    auto scanner = Lox::Scanner{source};

    auto tokens = scanner.scan_tokens();

    auto parser = Lox::Parser{tokens};
    auto expr = parser.parse();

    auto result = Lox::interpret(expr)->toString();

    ApprovalTests::Approvals::verify(result);
}

TEST(ParserTests, NumberLiteral) {
    runSource("3");
}

TEST(ParserTests, BooleanLiteral) {
    runSource("true");
}

TEST(ParserTests, Addition) {
    runSource("3 + 2");
}

TEST(ParserTests, ComparisonLessThan) {
    runSource("2 < 5");
}

TEST(ParserTests, ComparisonGreaterThan) {
    runSource("2 > 5");
}

TEST(ParserTests, ComparisonEqual) {
    runSource("10 == 10");
}