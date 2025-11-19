#ifndef __INTERPRETER_H_
#define __INTERPRETER_H_

#include <core/gtype.h>
#include <environment.h>
#include <expr.h>
#include <stmt.h>
#include <variant>

namespace ast {
    class AstInterpreter
    {
        public:
            core::gtype operator()(Assign* &);
            core::gtype operator()(Binary* &);
            core::gtype operator()(Unary* &);
            core::gtype operator()(Literal* &);
            core::gtype operator()(Grouping* &);
            core::gtype operator()(Variable* &);
            core::gtype operator()(Logical* &);
            core::gtype operator()(Call* &);
            core::gtype operator()(Noop* &);

            void operator()(Expression* &);
            void operator()(Print* &);
            void operator()(Block* &);
            void operator()(Var* &);
            void operator()(Function* &);
            void operator()(Void* &);
            void operator()(If* &);
            void operator()(While* &);

            void interpret(Expr expression) { 
                try {
                    core::gtype value = evaluate(expression);
                } catch (const std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            }

            void interpret(std::vector<Stmt> stmts) {
                try {
                    for (int i = 0; i < stmts.size(); i++)
                    {
                        execute(stmts[i]);
                    }
                } catch (const std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            }

            Environment* env = new Environment();

            void execute(Stmt &stmt);
            void executeBlock(std::vector<Stmt> stmts, Environment* env);
        private:
            core::gtype evaluate(Expr &expr);
    };
}

#endif
