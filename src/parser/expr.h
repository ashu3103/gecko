#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <gtype.h>
#include <token.h>

namespace ast {
	class Binary;
	class Unary;

	template <typename Derived>
	class Expr {
		public:
			template <typename R>
			class Visitor {
				virtual R visitBinaryExpr(Binary* expr) = 0;
				virtual R visitUnaryExpr(Unary* expr) = 0;
				virtual ~Visitor() = default;
			};
			virtual ~Expr() = default;
			template <typename R>
			R acceptVirtual(Visitor<R>& visitor) {
				static_cast<Derived*>(this)->accept(visitor);
			}
	};

	class Binary: public Expr<Binary> {
		public:
			Expr left;
			token::Token oper;
			Expr right;

			Binary(Expr left, token::Token oper, Expr right);

			template <typename R>
			R accept(Visitor<R>& vis);
	};

	class Unary: public Expr<Unary> {
		public:
			Expr mid;
			token::Token oper;

			Unary(Expr mid, token::Token oper);

			template <typename R>
			R accept(Visitor<R>& vis);
	};

}

#endif //__EXPR_H__
