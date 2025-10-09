#include <expr.h>

namespace ast {
	Binary::Binary(Expr left, token::Token oper, Expr right): left(left), oper(oper), right(right) {
	}

	template <typename R>
	R Binary::accept(Visitor<R>& visitor) {
		return visitor.visitBinaryExpr(this);
	}

	Unary::Unary(Expr mid, token::Token oper): mid(mid), oper(oper) {
	}

	template <typename R>
	R Unary::accept(Visitor<R>& visitor) {
		return visitor.visitUnaryExpr(this);
	}

}
