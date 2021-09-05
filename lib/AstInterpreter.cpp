//
// Created by berci on 2021. 08. 09..
//

#include <AstInterpreter.h>
#include <AstPrinter.h>
#include <TypeCast.h>
#include <RuntimeError.h>
#include <Statement.h>

#include <memory>

namespace Lox {
    std::shared_ptr<Value> interpret(Environment& env, const ExprPtr& expression) {
        if (LiteralPtr literal = std::dynamic_pointer_cast<Literal>(expression)) {
            return literal->value;
        }

        if (GroupingPtr grouping = std::dynamic_pointer_cast<Grouping>(expression)) {
            return interpret(env, grouping->expr);
        }

        if (UnaryPtr unary = std::dynamic_pointer_cast<Unary>(expression)) {
            auto op = unary->op.type_;
            auto operand = unary->operand;
            if (op == TokenType::MINUS) {
                auto leftEvaluated = interpret(env, unary->operand);
                auto left = as_number(leftEvaluated);
                return std::make_shared<Number>(-left->value);
            }

            if (op == TokenType::BANG) {
                auto leftEvaluated = interpret(env, unary->operand);
                auto leftTruthy = is_truthy(leftEvaluated);
                return std::make_shared<Boolean>(not leftTruthy);
            }

            throw RuntimeError(AstPrinter(unary));
        }

        if (BinaryPtr binary = std::dynamic_pointer_cast<Binary>(expression)) {
            auto left = interpret(env, binary->left);
            auto right = interpret(env, binary->right);
            auto op = binary->op;

            if (op.type_ == TokenType::MINUS) {
                auto l = as_number(left)->value;
                auto r = as_number(right)->value;
                return std::make_shared<Number>(l - r);
            }

            if (op.type_ == TokenType::STAR) {
                auto l = as_number(left)->value;
                auto r = as_number(right)->value;
                return std::make_shared<Number>(l * r);
            }

            if (op.type_ == TokenType::SLASH) {
                auto l = as_number(left)->value;
                auto r = as_number(right)->value;
                return std::make_shared<Number>(l / r);
            }

            if (op.type_ == TokenType::PLUS) {
                if (auto ls = std::dynamic_pointer_cast<String>(left)) {
                    if (auto rs = std::dynamic_pointer_cast<String>(right)) {
                        return std::make_shared<String>(ls->value + rs->value);
                    }
                }

                if (auto ln = std::dynamic_pointer_cast<Number>(left)) {
                    if (auto rn = std::dynamic_pointer_cast<Number>(right)) {
                        return  std::make_shared<Number>(ln->value + rn->value);
                    }
                }
            }

            if (op.type_ == TokenType::GREATER
                || op.type_ == TokenType::GREATER_EQUAL
                || op.type_ == TokenType::LESS
                || op.type_ == TokenType::LESS_EQUAL
            ) {
                auto l = as_number(left)->value;
                auto r = as_number(right)->value;
                switch (op.type_) {
                    case TokenType::GREATER: return std::make_shared<Boolean>(l > r);
                    case TokenType::GREATER_EQUAL: return std::make_shared<Boolean>(l >= r);
                    case TokenType::LESS: return std::make_shared<Boolean>(l < r);
                    case TokenType::LESS_EQUAL: return std::make_shared<Boolean>(l <= r);
                    default: break;
                }
            }

            if (op.type_ == TokenType::EQUAL_EQUAL) {
                return std::make_shared<Boolean>(is_equal(left, right));
            }

            if (op.type_ == TokenType::BANG_EQUAL) {
                std::make_shared<Boolean>(not is_equal(left, right));
            }

            throw RuntimeError(AstPrinter(binary));
        }

        if (auto var = std::dynamic_pointer_cast<Var>(expression)) {
            return env.get(var->name.lexeme_);
        }

        if (auto assignment = std::dynamic_pointer_cast<Assignment>(expression)) {
            auto value = interpret(env, assignment->value);
            env.define(assignment->name.lexeme_, value);
            return value;
        }

        assert(0 && "Unknown expression type");
    }

    std::shared_ptr<Value> interpret(Environment& env, const StatementPtr& statement, const Writer& writer) {
        if (auto printStatement = std::dynamic_pointer_cast<PrintStatement>(statement)) {
            auto expr = interpret(env, printStatement->expression);
            writer(expr->toString());
            return expr;
        }

        if (auto exprStatement = std::dynamic_pointer_cast<ExpressionStatement>(statement)) {
            return interpret(env, exprStatement->expression);
        }

        if (auto declaration = std::dynamic_pointer_cast<VariableStatement>(statement)) {
            auto value = interpret(env, declaration->initializer);
            env.define(declaration->name.lexeme_, value);
            return value;
        }

        if (auto block = std::dynamic_pointer_cast<BlockStatement>(statement)) {
            auto nextEnv = Environment(std::make_shared<Environment>(env));
            auto result = static_cast<std::shared_ptr<Value>>(nullptr);

            for (auto& stmt : block->statements) {
                result = interpret(nextEnv, stmt, writer);
            }

            return result;
        }

        assert(0 && "Unknown Statement subclass");
    }
}
