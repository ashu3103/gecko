#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <variant>
#include <token.h>

namespace ast {
	struct Assign;
	struct Binary;
	struct Unary;
	struct Grouping;
	struct Literal;
	struct Variable;
	struct Noop;

	using Expr = std::variant<Assign*, Binary*, Unary*, Grouping*, Literal*, Variable*, Noop*>;

	struct Assign {
		token::Token name;
		Expr value;

		Assign(token::Token name, Expr value): name(name), value(value) {}
	};

	struct Binary {
		Expr lhs;
		token::Token oper;
		Expr rhs;

		Binary(Expr lhs, token::Token oper, Expr rhs): lhs(lhs), oper(oper), rhs(rhs) {}
	};

	struct Unary {
		token::Token oper;
		Expr rhs;

		Unary(token::Token oper, Expr rhs): oper(oper), rhs(rhs) {}
	};

	struct Grouping {
		Expr expr;

		Grouping(Expr expr): expr(expr) {}
	};

	struct Literal {
		std::string value;

		Literal(std::string value): value(value) {}
	};

	struct Variable {
		token::Token name;

		Variable(token::Token name): name(name) {}
	};

	struct Noop {

		Noop() {}
	};

}

#endif //__EXPR_H__
