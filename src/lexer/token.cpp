#include <token.h>

namespace token
{
    /* Function definitions for Token */
    Token::Token(TokenType type, std::string tok, size_t s_off, size_t e_off): pos(s_off, e_off)
    {
        this->type = type;
        this->tok = tok;
    }
    
    Token::~Token() {}
}
