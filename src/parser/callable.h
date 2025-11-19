#ifndef __CALLABLE_H_
#define __CALLABLE_H_

#include <core/gtype.h>
#include <environment.h>
#include <interpreter.h>
#include <stmt.h>

#include <iostream>
#include <vector>

namespace ast {
    class funtype {
        public:
            Function declaration;
            funtype(Function d);
            int Arity();
            core::gtype Call(AstInterpreter interpreter, std::vector<core::gtype> args);
    };
}

#endif