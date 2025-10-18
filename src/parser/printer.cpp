#include <printer.h>

namespace ast {
    std::string AstPrinter::operator()(Binary* &expr)
    {
        return parenthesize(expr->oper.tok, expr->lhs, expr->rhs);
    }

    std::string AstPrinter::operator()(Grouping* &expr)
    {
        return parenthesize("group", expr->expr);
    }

    std::string AstPrinter::operator()(Unary* &expr)
    {
        return parenthesize(expr->oper.tok, expr->rhs);
    }

    std::string AstPrinter::operator()(Literal* &expr)
    {
        return parenthesize(expr->value);
    }

    std::string AstPrinter::operator()(Noop* &expr)
    {
        return parenthesize("noop");
    }

    std::string AstPrinter::print(Expr &expr)
    {
        return std::visit(*this, expr);
    }

}