#ifndef __STMT_H__
#define __STMT_H__

#include <iostream>
#include <variant>
#include <token.h>
#include <expr.h>

namespace ast {
	struct Expression;
	struct Print;
	struct Var;

	using Stmt = std::variant<Expression*, Print*, Var*>;

	struct Expression {
		Expr expr;

		Expression(Expr expr): expr(expr) {}
	};

	struct Print {
		Expr expr;

		Print(Expr expr): expr(expr) {}
	};

	struct Var {
		token::Token name;
		Expr initializer;

		Var(token::Token name, Expr initializer): name(name), initializer(initializer) {}
	};

}

#endif //__STMT_H__
