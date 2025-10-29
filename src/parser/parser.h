#include <iostream>
#include <token.h>
#include <expr.h>
#include <stmt.h>
#include <vector>

using namespace token;

const std::vector<TokenType> stopset = {
    TokenType::_BREAK,
    TokenType::_CONTINUE,
    TokenType::_FOR,
    TokenType::_IF,
    TokenType::_RETURN,
    TokenType::_SEMICOLON,
    TokenType::_VAR,
    TokenType::_WHILE,
};

namespace ast {
    class Parser {
        public:
            std::vector<Token> tokens = {};
            int current = 0;
            bool fnest = false;
            Parser(std::vector<Token> t);
            ~Parser();

            bool Match(std::vector<TokenType> matchList);
            void Advance(std::vector<TokenType> followList);

            bool Got(TokenType type);
            bool Want(TokenType type);
            
            Token Previous();
            Token Current();
            Expr Expression();
            Stmt Statement();
    };
}