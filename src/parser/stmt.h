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
	struct If;
	struct While;
	struct Void;

	using Stmt = std::variant<Expression*, Print*, Var*, Block*, If*, While*, Void*>;

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

	struct If {
		Expr condition;
		Stmt thenBranch;
		Stmt elseBranch;

		If(Expr condition, Stmt thenBranch, Stmt elseBranch): condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
	};

	struct While {
		Expr condition;
		Stmt body;

		While(Expr condition, Stmt body): condition(condition), body(body) {}
	};

	struct Void {

		Void() {}
	};

}

#endif //__STMT_H__
