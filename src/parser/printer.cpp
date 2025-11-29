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

    std::string AstPrinter::operator()(Logical* &expr)
    {
        return parenthesize(expr->oper.tok, expr->lhs, expr->rhs);
    }

    std::string AstPrinter::operator()(Call* &expr)
    {
        return parenthesize("call", expr->callee);
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

    std::string AstPrinter::operator()(Block* &stmt) {
        std::string out = "(block ";

        for (int i = 0; i < stmt->statements.size(); i++)
        {
            out += print(stmt->statements[i]);
        }

        out += ")";
        return out;
    }

    std::string AstPrinter::operator()(If* &stmt) {
        if (core::is_type<Void*>(stmt->elseBranch))
        {
            return parenthesize2("if", stmt->condition, stmt->thenBranch);
        }
        else
        {
            return parenthesize2("if-else", stmt->condition, stmt->thenBranch, stmt->elseBranch);
        }
    }

    std::string AstPrinter::operator()(While* &stmt) {
        return parenthesize2("while", stmt->condition, stmt->body);
    }

    std::string AstPrinter::operator()(Function* &stmt) {
        std::string out = "(fun " + stmt->name.tok + "(";

        for (int i = 0; i < stmt->params.size(); i++)
        {
            if (i != 0) out += ", ";
            out += stmt->params[i].tok;
        }

        out += ") (block ";

        for (int i = 0; i < stmt->body.size(); i++)
        {
            if (i != 0) out += " ";
            out += print(stmt->body[i]);
        }

        out += ")";
        return out;
    }

    std::string AstPrinter::operator()(Return* &stmt) {
        return parenthesize2("return", stmt->value);
    }

    std::string AstPrinter::operator()(Void* &stmt)
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