#include <printer.h>

namespace ast {
    std::string AstPrinter::visitBinaryExpr(Binary<std::string>* expr)
    {
        return parenthesize(expr->oper.tok, expr->left, expr->right);
    }

    std::string AstPrinter::visitGroupingExpr(Grouping<std::string>* expr)
    {
        return parenthesize("group", expr->expr);
    }

    std::string AstPrinter::visitUnaryExpr(Unary<std::string>* expr)
    {
        return parenthesize(expr->oper.tok, expr->right);
    }

    std::string AstPrinter::visitLiteralExpr(Literal<std::string>* expr)
    {
        return parenthesize(core::stringify(expr->lit));
    }
}