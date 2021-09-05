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
    auto env = std::make_shared<Lox::Environment>();

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

TEST(ParserTests, ComparisonNotEqual) {
    runSource("10 != 10;");
}

TEST(ParserTests, AllComparinons) {
    auto source = R"(
        print 1 < 2;
        print 1 <= 1;
        print 1 <= 2;
        print 1 > 2;
        print 1 >= 2;
        print 2 >= 2;
        print 1 == 1;
        print 1 != 1;
    )";

    runSource(source);
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

TEST(ParserTests, UpdateInPlace) {
    auto source = R"(
        var a = 4;
        a = a + 1;
        print a;
    )";
    runSource(source);
}

TEST(ParserTests, MultipleBlocks) {
    auto source = R"(
        {
            var a = 4;
            a = a + 3;
            print a;
        }

        {
            var b = 11;
            b = b * 10;
            print b;
        }

        print "done";
    )";

    runSource(source);
}

TEST(ParserTests, IfElseStatement) {
    auto source = R"(
        if (true) {
            print "true";
        } else {
            print "false";
        }

        if (0) {
            print "0 is truthy";
        } else {
            print "0 is falsy";
        }
    )";

    runSource(source);
}

TEST(ParserTests, IfStatement) {
    auto source = R"(
        var a = 1;
        if (a) {
            print "a is truthy";
        }

        a = a - 1;

        if (a) {
            print "a is still truthy";
        } else {
            print "a is falsy";
        }
    )";

    runSource(source);
}

TEST(ParserTests, WhileLoop) {
    auto source = R"(
        var a = 0;
        while (a < 10) {
            print a;
            a = a + 1;
        }
        print "done!";
    )";

    runSource(source);
}

TEST(ParserTests, ForLoop) {
    auto source = R"(
        var result = 1;
        for (var i = 1; i <= 5; i = i + 1) {
            result = result * i;
        }
        print result;
    )";
    runSource(source);
}

TEST(ParserTests, FunctionDefinition) {
    auto source = R"(
        function add(a, b) {
            a + b;
        }
    )";
    runSource(source);
}

TEST(ParserTests, FunctionCall) {
    auto source = R"(
        function add(a, b) {
            a + b;
        }
        add(4, 5);
    )";
    runSource(source);
}

TEST(ParserTests, Increment) {
    auto source = R"(
        function inc(n) {
            n + 1;
        }
        inc(5);
    )";
    runSource(source);
}

TEST(ParserTests, Fibonacci) {
    auto source = R"(
        function fib(n) {
            var a = 0;
            var b = 1;
            for (var i = 0; i < n - 2; i = i + 1) {
                var temp = b;
                b = a + b;
                a = temp;
            }
            b;
        }
        print "The 5th Fibonacci number is:";
        print fib(5);
        print "The 8th Fibonacci number is:";
        print fib(8);
    )";
    runSource(source);
}