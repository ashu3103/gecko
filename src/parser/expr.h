#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <variant>
#include <token.h>

namespace ast {
	struct Binary;
	struct Unary;
	struct Grouping;
	struct Literal;

	using Expr = std::variant<Binary*, Unary*, Grouping*, Literal*>;

	struct Binary {
		Expr lhs;
		token::Token oper;
		Expr rhs;

		Binary(Expr lhs, token::Token oper, Expr rhs): lhs(lhs), oper(oper), rhs(rhs) {
		}

	};

	struct Unary {
		token::Token oper;
		Expr rhs;

		Unary(token::Token oper, Expr rhs): oper(oper), rhs(rhs) {
		}

	};

	struct Grouping {
		Expr expr;

		Grouping(Expr expr): expr(expr) {
		}

	};

	struct Literal {
		std::string value;

		Literal(std::string value): value(value) {
		}

	};

}

#endif //__EXPR_H__
