#include <interpreter.h>
#include <callable.h>
#include <error/errors.h>

bool IsTruthy(core::gtype expr) {
    if (core::is_type<bool>(expr))
    {
        return !std::get<bool>(expr);
    }
    if (core::is_type<std::monostate>(expr))
    {
        return false;
    }
    return true;
}

template <typename... Args>
std::string Format(const Args&... args) {
    std::string out = "";
    (out = out + args);
    return out;
}

namespace ast {
    core::gtype AstInterpreter::operator()(Noop* &expr) {
        return std::monostate{};
    }

    core::gtype AstInterpreter::operator()(Logical* &expr) {
        core::gtype lhs = evaluate(expr->lhs);

        /* implement short circuiting */
        if (expr->oper.type == _OR) {
            if (IsTruthy(lhs)) return lhs;
        } else if (expr->oper.type == _AND) {
            if (!IsTruthy(lhs)) return lhs;
        }

        return evaluate(expr->rhs);
    }

    core::gtype AstInterpreter::operator()(Literal* &expr) {
        return core::get_value(expr->value);
    }

    core::gtype AstInterpreter::operator()(Grouping* &expr) {
        return evaluate(expr->expr);
    }

    core::gtype AstInterpreter::operator()(Unary* &expr) {
        core::gtype rhs = evaluate(expr->rhs);

        switch (expr->oper.type)
        {
            case TokenType::_DASH:
                if (core::is_type<double>(rhs))
                {
                    return -1 * std::get<double>(rhs);
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting a number"));
                break;
            case TokenType::_BANG:
                return !IsTruthy(rhs);
            default:
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_CHARACTER, expr->oper.pos, "expecting a '!' or '-'"));
                break;
        }

        return std::monostate{};
    }

    core::gtype AstInterpreter::operator()(Binary* &expr) {
        core::gtype lhs = evaluate(expr->lhs);
        core::gtype rhs = evaluate(expr->rhs);

        switch (expr->oper.type)
        {
            case TokenType::_PLUS:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) + std::get<double>(rhs));
                }
                if (core::is_type<std::string>(lhs) && core::is_type<std::string>(rhs))
                {
                    return std::get<std::string>(lhs) + std::get<std::string>(rhs);
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number or string type"));
            case TokenType::_DASH:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) - std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_STAR:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) * std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_SLASH:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    if (std::get<double>(rhs) == 0)
                    {
                        throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::DIVISION_BY_ZERO, expr->oper.pos, "division by zero is not allowed"));
                    }
                    else
                    {
                        return (std::get<double>(lhs) / std::get<double>(rhs));
                    }
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_GREATER:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) > std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_GREATER_EQUAL:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) >= std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_LESS:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) < std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_LESS_EQUAL:
                if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
                {
                    return (std::get<double>(lhs) <= std::get<double>(rhs));
                }
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
            case TokenType::_BANG_EQUAL:
                return !core::equals(lhs, rhs);
            case TokenType::_EQUAL_EQUAL:
                return core::equals(lhs, rhs);
            default:
                std::string msg = "expecting a binary operator but got " + expr->oper.tok;
                throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_CHARACTER, expr->oper.pos, msg));
        }

        return std::monostate{};
    }

    core::gtype AstInterpreter::operator()(Assign* &expr) {
        core::gtype value = evaluate(expr->value);
        env->Assign(expr->name, value);
        return value;
    }

    core::gtype AstInterpreter::operator()(Call* &expr) {
        core::gtype callee = evaluate(expr->callee);
        std::vector<core::gtype> arguments = {};
        for (int i = 0; i < expr->arguments.size(); i++)
        {
            arguments.push_back(evaluate(expr->arguments[i]));
        }

        // check if the callee is callable
        if (!core::is_type<funtype*>(callee))
        {
            throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->paren.pos, "can only call functions"));
        }

        funtype* function = std::get<funtype*>(callee);

        if (function->Arity() != arguments.size())
        {
            int arity = function->Arity();
            throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::GENERIC_ERROR, expr->paren.pos, Format("Expected ", std::to_string(arity), " arguments but got ", std::to_string(arguments.size()), ".")));
        }

        return function->Call(*this, arguments);
    }

    core::gtype AstInterpreter::operator()(Variable* &expr) {
        return env->Get(expr->name);
    }

    void AstInterpreter::operator()(Print* &stmt) {
        core::gtype value = evaluate(stmt->expr);
        std::cout << core::stringify(value) << std::endl;
    }

    void AstInterpreter::operator()(Expression* &stmt) {
        evaluate(stmt->expr);
    }

    void AstInterpreter::operator()(Void* &) {
        return;
    }

    void AstInterpreter::operator()(Var* &stmt) {
        core::gtype value = std::monostate{};
        if (!core::is_type<Noop*>(stmt->initializer))
        {
            value = evaluate(stmt->initializer);
        }

        env->Define(stmt->name.tok, value);
    }

    void AstInterpreter::operator()(If* &stmt) {
        if (IsTruthy(evaluate(stmt->condition)))
        {
            execute(stmt->thenBranch);
        }
        else if (!core::is_type<Void*>(stmt->elseBranch))
        {
            execute(stmt->elseBranch);
        }
        return;
    }

    void AstInterpreter::operator()(While* &stmt) {
        while (IsTruthy(evaluate(stmt->condition)))
        {
            execute(stmt->body);
        }
    }

    void AstInterpreter::operator()(Block* &stmt) {
        executeBlock(stmt->statements, new Environment(this->env));
    }

    void AstInterpreter::operator()(Function* &stmt) {
        funtype* function = new funtype(*stmt);
        env->Define(stmt->name.tok, function);
        return;
    }

    core::gtype AstInterpreter::evaluate(Expr &expr) {
        return std::visit(*this, expr);
    }

    void AstInterpreter::execute(Stmt &stmt) {
        std::visit(*this, stmt);
    }

    void AstInterpreter::executeBlock(std::vector<Stmt> stmts, Environment* env) {
        Environment* previous = this->env;
        try {
            this->env = env;
            for (int i = 0; i < stmts.size(); i++)
            {
                execute(stmts[i]);
            }

            this->env = previous;
        } catch (...) {
            this->env = previous;
        }
    }
}
