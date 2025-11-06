#ifndef __STMT_H__
#define __STMT_H__

#include <iostream>
#include <variant>
#include <vector>
#include <token.h>
#include <expr.h>

namespace ast {
	struct Expression;
	struct Print;
	struct Var;
	struct Block;
	struct Void;

	using Stmt = std::variant<Expression*, Print*, Var*, Block*, Void*>;

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

	struct Block {
		std::vector<Stmt> statements;

		Block(std::vector<Stmt> statements): statements(statements) {}
	};

	struct Void {

		Void() {}
	};

}

#endif //__STMT_H__
