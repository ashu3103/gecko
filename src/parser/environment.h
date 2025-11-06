#include <iostream>
#include <map>
#include <string>

#include <core/gtype.h>
#include <error/errors.h>
#include <token.h>

namespace ast {
    class Environment {
        public:
            std::map<std::string, core::gtype> values;

            Environment();
            ~Environment();
            void Define(std::string name, core::gtype value);
            core::gtype Get(token::Token tok);
            void Assign(token::Token tok, core::gtype value);
    };
}