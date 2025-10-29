#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <position/position.h>

#include <iostream>
#include <string>
#include <unordered_map>

enum TokenType {
    // invalid token type
    _XXX,

    // single character operator
    _PLUS,         // +
    _DASH,         // -
    _STAR,         // *
    _SLASH,        // /
    _LEFT_PAREN,   // (
    _RIGHT_PAREN,  // )
    _LEFT_BRACE,   // {
    _RIGHT_BRACE,  // }
    _COMMA,        // ,
    _SEMICOLON,    // ;
    _DOT,          // .
    _BANG,         // !
    _EQUAL,        // =
    _GREATER,      // >
    _LESS,         // <

    //two character operator
    _BANG_EQUAL,   // !=
    _EQUAL_EQUAL,  // ==
    _GREATER_EQUAL,// >=
    _LESS_EQUAL,   // <=

    // literals
    _IDENTIFIER,
    _STRING,
    _NUMBER,

    // keywords
    _OR,
    _AND,
    _NIL,
    _TRUE,
    _FALSE,
    _VAR,
    _FUN,
    _IF,
    _ELSE,
    _FOR,
    _WHILE,
    _BREAK,
    _CONTINUE,
    _CLASS,
    _THIS,
    _SUPER,
    _RETURN,
    _PRINT,

    _EOF,
};

const std::vector<std::string> operatorMap = {"+", "-", "*", "/", "(", ")", "{", "}", ",", ";", ".", "!", "=", ">", "<", "!=", "==", ">=", "<="};

const std::unordered_map<std::string, TokenType> keywordMap = {
    {"or", _OR},
    {"and", _AND},
    {"nil", _NIL},
    {"true", _TRUE},
    {"false", _FALSE},
    {"var", _VAR},
    {"fun", _FUN},
    {"if", _IF},
    {"else", _ELSE},
    {"for", _FOR},
    {"while", _WHILE},
    {"break", _BREAK},
    {"continue", _CONTINUE},
    {"class", _CLASS},
    {"this", _THIS},
    {"super", _SUPER},
    {"return", _RETURN},
    {"print", _PRINT},
};

namespace token {
        class Token {
        public:
            TokenType type;
            std::string tok;
            position::Pos pos;
        
        Token(TokenType type, std::string tok, size_t s_off, size_t e_off);
        ~Token();

        static bool IsKeyword(std::string tok)
        {
            if (keywordMap.find(tok) != keywordMap.end()) {
                return true;
            }
            return false;
        }

        static bool IsOperator(TokenType type)
        {
            if (type > 0 && type < 20) return true;
            return false;
        }
    };
}

#endif // __TOKEN_H__