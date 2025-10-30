#include <printer.h>
#include <core/gtype.h>

namespace ast {
    std::string AstPrinter::operator()(Assign* &expr)
    {
        return parenthesize2("=", expr->name.tok, expr->value);
    }

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

    std::string AstPrinter::operator()(Variable* &expr)
    {
        return parenthesize(expr->name.tok);
    }

    std::string AstPrinter::operator()(Expression* &stmt)
    {
        return print(stmt->expr);
    }

    std::string AstPrinter::operator()(Print* &stmt)
    {
        return parenthesize2("print", stmt->expr);
    }

    std::string AstPrinter::operator()(Var* &stmt)
    {
        if (core::is_type<Noop*>(stmt->initializer)) // var declaration doesn't have an initializer
        {
            return parenthesize2("var", stmt->name.tok);
        }

        return parenthesize2("var", stmt->name.tok, stmt->initializer);
    }

    std::string AstPrinter::operator()(Void* &expr)
    {
        return "void";
    }

    std::string AstPrinter::print(Expr &expr)
    {
        return std::visit(*this, expr);
    }

    std::string AstPrinter::print(Stmt &stmt)
    {
        return std::visit(*this, stmt);
    }
}