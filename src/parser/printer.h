#include <iostream>
#include <expr.h>
#include <string>

namespace ast {
    class AstPrinter
    {
        public:
            std::string operator()(Binary* &);
            std::string operator()(Unary*&);
            std::string operator()(Literal*&);
            std::string operator()(Grouping*&);
            std::string operator()(Noop*&);

            std::string print(Expr &expr);

        private:
            // Helper to make (operator left right) style strings
            template <typename... Args>
            std::string parenthesize(const std::string& name, Args... exprs) {
                std::string result = "(" + name;
                ((result += " " + print(exprs)), ...);
                result += ")";
                return result;
            }

    };
}
