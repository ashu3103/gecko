// #include <interpreter.h>
// #include <error/errors.h>

// namespace ast {
//     core::gtype AstInterpreter::operator()(Literal* &expr) {
//         return core::get_value(expr->value);
//     }

//     core::gtype AstInterpreter::operator()(Grouping* &expr) {
//         return std::visit(*this, expr->expr);
//     }

//     core::gtype AstInterpreter::operator()(Unary* &expr) {
//         core::gtype rhs = std::visit(*this, expr->rhs);

//         switch (expr->oper.type)
//         {
//             case TokenType::_DASH:
//                 if (core::is_type<double>(rhs))
//                 {
//                     return -1 * std::get<double>(rhs);
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting a number"));
//                 break;
//             case TokenType::_BANG:
//                 if (core::is_type<bool>(rhs))
//                 {
//                     return !std::get<bool>(rhs);
//                 }
//                 if (core::is_type<std::monostate>(rhs))
//                 {
//                     return true;
//                 }
//                 return false;
//             default:
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_CHARACTER, expr->oper.pos, "expecting a '!' or '-'"));
//                 break;
//         }

//         return std::monostate{};
//     }

//     core::gtype AstInterpreter::operator()(Binary* &expr) {
//         core::gtype lhs = std::visit(*this, expr->lhs);
//         core::gtype rhs = std::visit(*this, expr->rhs);

//         switch (expr->oper.type)
//         {
//             case TokenType::_PLUS:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) + std::get<double>(rhs));
//                 }
//                 if (core::is_type<std::string>(lhs) && core::is_type<std::string>(rhs))
//                 {
//                     return std::get<std::string>(lhs) + std::get<std::string>(rhs);
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number or string type"));
//             case TokenType::_DASH:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) - std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_STAR:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) * std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_SLASH:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     if (std::get<double>(rhs) == 0)
//                     {
//                         throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::DIVISION_BY_ZERO, expr->oper.pos, "division by zero is not allowed"));
//                     }
//                     else
//                     {
//                         return (std::get<double>(lhs) / std::get<double>(rhs));
//                     }
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_GREATER:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) > std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_GREATER_EQUAL:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) >= std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_LESS:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) < std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_LESS_EQUAL:
//                 if (core::is_type<double>(lhs) && core::is_type<double>(rhs))
//                 {
//                     return (std::get<double>(lhs) <= std::get<double>(rhs));
//                 }
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_OPERAND_TYPE, expr->oper.pos, "expecting both the operands of number type"));
//             case TokenType::_BANG_EQUAL:
//                 return !core::equals(lhs, rhs);
//             case TokenType::_EQUAL_EQUAL:
//                 return core::equals(lhs, rhs);
//             default:
//                 std::string msg = "expecting a binary operator but got " + expr->oper.tok;
//                 throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::INVALID_CHARACTER, expr->oper.pos, msg));
//         }

//         return std::monostate{};
//     }
// }
