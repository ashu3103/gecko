#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <variant>
#include <token.h>

namespace ast {
	struct Binary;
	struct Unary;
	struct Literal;
	struct Grouping;

	using Expr = std::variant<Binary*, Unary*, Literal*, Grouping*>;

	struct Binary {
		Expr lhs;
		token::Token oper;
		Expr rhs;
		Binary(Expr l, token::Token o, Expr r)
        : lhs(l), oper(o), rhs(r) {}
	};

	struct Unary {
		token::Token oper;
		Expr rhs;
		Unary(token::Token o, Expr r)
		: oper(o), rhs(r) {}
	};

	struct Literal {
		std::string value;
		explicit Literal(std::string v) : value(v) {}
	};

	struct Grouping {
		Expr expr;
		explicit Grouping(Expr e) : expr(e) {}
	};
}

#endif //__EXPR_H__
