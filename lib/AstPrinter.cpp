//
// Created by berci on 2021. 08. 07..
//

#include <AstPrinter.h>
#include <Expr.h>

namespace Lox {

    std::string AstPrinter(const std::shared_ptr<Expr>& expr) {
        if (BinaryPtr binary = std::dynamic_pointer_cast<Binary>(expr)) {
            auto left = AstPrinter(binary->left);
            auto right = AstPrinter(binary->right);
            auto op = binary->op.lexeme_;

            return "(" + op + " " + left + " " + right + ")";
        }

        if (UnaryPtr unary = std::dynamic_pointer_cast<Unary>(expr)) {
            auto op = unary->op.lexeme_;
            auto operand = AstPrinter(unary->operand);

            return "(" + op + " " + operand + ")";
        }

        if (GroupingPtr grouping = std::dynamic_pointer_cast<Grouping>(expr)) {
            auto group = AstPrinter(grouping->expr);
            return "(" + group + ")";
        }

        if (LiteralPtr literal = std::dynamic_pointer_cast<Literal>(expr)) {
            return literal->value->toString();
        }

        assert(0 && "Unknown Expr subclass");
    }

}

