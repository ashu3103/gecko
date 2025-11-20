#include <callable.h>

namespace ast {
    funtype::funtype(Function d): declaration(d) {}

    int funtype::Arity() {
        return (int)declaration.params.size();
    }

    core::gtype funtype::Call(AstInterpreter interpreter, std::vector<core::gtype> args) {
        Environment* e = new Environment(interpreter.env);

        for (int i = 0; i < args.size(); i++)
        {
            e->Define(declaration.params[i].tok, args[i]);
        }

        try {
            interpreter.executeBlock(declaration.body, e);
        } catch (ReturnE returnValue) {
            return returnValue.value;
        }

        return std::monostate{};
    }

}