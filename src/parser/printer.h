#include <iostream>
#include <expr.h>
#include <string>

namespace ast {
    class AstPrinter: public Visitor<std::string>
    {
        public:
            std::string visitBinaryExpr(Binary<std::string>* expr) override;

            std::string visitUnaryExpr(Unary<std::string>* expr) override;

            std::string visitGroupingExpr(Grouping<std::string>* expr) override;

            std::string visitLiteralExpr(Literal<std::string>* expr) override;

        private:
            // Helper to make (operator left right) style strings
            template <typename... Args>
            std::string parenthesize(const std::string& name, Args... exprs) {
                std::string result = "(" + name;
                ((result += " " + print(exprs)), ...);
                result += ")";
                return result;
            }

            std::string print(Expr<std::string>* expr)
            {
                return expr->accept(*this);
            }
    };
}
