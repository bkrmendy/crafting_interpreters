//
// Created by berci on 2021. 08. 28..
//

#include "gtest/gtest.h"
#include "ApprovalTests.hpp"

#include <Scanner.h>
#include <Parser.h>
#include <AstInterpreter.h>
#include <utils.h>

#include <numeric>
#include <string>

void runSource(const std::string& source) {
    auto scanner = Lox::Scanner{source};

    auto tokens = scanner.scan_tokens();

    auto parser = Lox::Parser{tokens};
    auto statements = parser.parse();

    auto output = std::vector<std::string>{};
    auto env = Lox::Environment{};

    auto result = std::shared_ptr<Lox::Value>(nullptr);

    for (auto& statement : statements) {
        result = Lox::interpret(env, statement, [&output](const std::string& line) { output.push_back(line + '\n'); });
    }

    auto dump = std::reduce(output.begin(), output.end()) + result->toString();

    ApprovalTests::Approvals::verify(dump);
}

TEST(ParserTests, NumberLiteral) {
    runSource("3;");
}

TEST(ParserTests, BooleanLiteral) {
    runSource("true;");
}

TEST(ParserTests, Addition) {
    runSource("3 + 2;");
}

TEST(ParserTests, ComparisonLessThan) {
    runSource("2 < 5;");
}

TEST(ParserTests, ComparisonGreaterThan) {
    runSource("2 > 5;");
}

TEST(ParserTests, ComparisonEqual) {
    runSource("10 == 10;");
}

TEST(ParserTests, Parentheses) {
    runSource("3 * (2 + 2);");
}

TEST(ParserTests, PrintStatement) {
    runSource("print 33;");
}

TEST(ParserTests, PrintAString) {
    auto source = R"(print "Hello World!";)";
    runSource(source);
}

TEST(ParserTests, MultiLineStatement) {
    auto source = R"(
        print 4;
        print 5;
        4 + 5;
    )";
    runSource(source);
}

TEST(ParserTests, Declaration) {
    auto source = R"(
        var x = 3;
        print x;
    )";
    runSource(source);
}

TEST(ParserTests, ExpressionInDeclaration) {
    auto source = R"(
        var x = (10 + 2) * 3;
        print x;
    )";
    runSource(source);
}

TEST(ParserTests, Assignment) {
    auto source = R"(
        var x = 5;
        x = 6;
        print x;
    )";
    runSource(source);
}

TEST(ParserTests, Block) {
    auto source = R"(
        var x = 5;
        print x;
        {
            var x = 3;
            print x;
        }
    )";
    runSource(source);
}