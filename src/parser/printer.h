#include <iostream>
#include <expr.h>
#include <stmt.h>
#include <string>

namespace ast {
    class AstPrinter
    {
        public:
            std::string operator()(Assign* &);
            std::string operator()(Binary* &);
            std::string operator()(Unary* &);
            std::string operator()(Literal* &);
            std::string operator()(Grouping* &);
            std::string operator()(Variable* &);
            std::string operator()(Noop* &);

            std::string operator()(Expression* &);
            std::string operator()(Print* &);
            std::string operator()(Var* &);
            std::string operator()(Void* &);

            /* function overloading */
            std::string print(Expr &expr);
            std::string print(Stmt &stmt);
        private:
            // Helper to make (operator left right) style strings
            template <typename... Args>
            std::string parenthesize(const std::string& name, Args... exprs) {
                std::string result = "(" + name;
                ((result += " " + print(exprs)), ...);
                result += ")";
                return result;
            }

            template <typename... Args>
            std::string parenthesize2(const std::string& name, Args... args) {
                std::string result = "(" + name;
                this->transform(result, args...);
                result += ")";
                return result;
            }

            template <typename... Args>
            void transform(std::string &result, Args... args)
            {
                // fold expression
                (([&result, this](auto&& x) {
                    using T = std::decay_t<decltype(x)>;

                    if constexpr (std::is_same_v<T, Expr>) {
                        result += " " + this->print(x);   // assuming print(Expr) -> std::string
                    }
                    else if constexpr (std::is_same_v<T, Stmt>) {
                        result += " " + this->print(x);   // assuming print(Stmt) -> std::string
                    }
                    else if constexpr (std::is_same_v<T, std::string>) {
                        result += " " + x;
                    }
                    else {
                        std::cout << "unknown type\n";
                    }
                }(std::forward<Args>(args))), ...);
            }

    };
}
