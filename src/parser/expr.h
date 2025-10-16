#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <gtype.h>
#include <token.h>

namespace ast {
	template <typename R>
	class Binary;
	template <typename R>
	class Unary;
	template <typename R>
	class Grouping;
	template <typename R>
	class Literal;

	template <typename R>
	class Visitor {
		public:
			virtual R visitBinaryExpr(Binary<R>* expr) = 0;
			virtual R visitUnaryExpr(Unary<R>* expr) = 0;
			virtual R visitGroupingExpr(Grouping<R>* expr) = 0;
			virtual R visitLiteralExpr(Literal<R>* expr) = 0;
			virtual ~Visitor() = default;
	};
	template <typename R>
	class Expr {
		public:
			virtual ~Expr() = default;
			virtual R accept(Visitor<R>& visitor) = 0;
	};

	template <typename R>
	class Binary: public Expr<R> {
		public:
			Expr<R>* left;
			token::Token oper;
			Expr<R>* right;

			Binary(Expr<R>* left, token::Token oper, Expr<R>* right): left(left), oper(oper), right(right) {
			}

			R accept(Visitor<R>& visitor) override {
				return visitor.visitBinaryExpr(this);
			}
	};

	template <typename R>
	class Unary: public Expr<R> {
		public:
			Expr<R>* right;
			token::Token oper;

			Unary(Expr<R>* right, token::Token oper): right(right), oper(oper) {
			}

			R accept(Visitor<R>& visitor) override {
				return visitor.visitUnaryExpr(this);
			}
	};

	template <typename R>
	class Grouping: public Expr<R> {
		public:
			Expr<R>* expr;

			Grouping(Expr<R>* expr): expr(expr) {
			}

			R accept(Visitor<R>& visitor) override {
				return visitor.visitGroupingExpr(this);
			}
	};

	template <typename R>
	class Literal: public Expr<R> {
		public:
			core::gtype lit;

			Literal(core::gtype lit): lit(lit) {
			}

			R accept(Visitor<R>& visitor) override {
				return visitor.visitLiteralExpr(this);
			}
	};

}

#endif //__EXPR_H__
