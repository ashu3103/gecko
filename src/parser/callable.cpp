#include <callable.h>

namespace ast {
    funtype::funtype(Function d): declaration(d) {}

    int funtype::Arity() {
        declaration.params.size();
    }

    core::gtype funtype::Call(AstInterpreter interpreter, std::vector<core::gtype> args) {
        Environment* e = new Environment(interpreter.env);

        for (int i = 0; i < args.size(); i++)
        {
            e->Define(declaration.params[i].tok, args[i]);
        }

        interpreter.executeBlock(declaration.body, e);
        return std::monostate{};
    }

}