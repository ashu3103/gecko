#include <environment.h>

namespace ast {
    Environment::Environment() {}

    Environment::~Environment() {
        values.clear();
    }

    void Environment::Define(std::string name, core::gtype value) {
        values.insert({name, value});
    }

    core::gtype Environment::Get(token::Token tok) {
        if (values.find(tok.tok) != values.end()) {
            return values.at(tok.tok);
        }

        std::string msg = "Undefined variable " + tok.tok + ".";
        throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::UNDEFINED_VARIABLE, tok.pos, msg));
    }

    void Environment::Assign(token::Token tok, core::gtype value) {
        if (values.find(tok.tok) != values.end()) {
            values.insert({tok.tok, value});
            return;
        }

        std::string msg = "Undefined variable " + tok.tok + ".";
        throw std::runtime_error(errors::GenerateRuntimeError(errors::ErrorType::UNDEFINED_VARIABLE, tok.pos, msg));
    }
}