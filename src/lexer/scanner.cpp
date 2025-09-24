#include "scanner.h"

namespace scanner {
    /* Function definitions for Token */
    Token::Token(TokenType type, std::string tok, size_t s_off, size_t e_off): pos(s_off, e_off)
    {
        this->type = type;
        this->tok = tok;
    }

    Token::~Token() {}

    // bool Token::IsKeyword(std::string tok)

    /* Function definitions for Scanner  */
    Scanner::Scanner(std::fstream&& in): src(std::move(in))
    {
        start_off = 0;
        end_off = 0;
        this->tokens.clear();
    }

    Scanner::~Scanner() {}

    bool Scanner::NextToken()
    {
        src.Stop();
        /* skip whitespaces */
        while(src.chr == ' ' || src.chr == '\t' || src.chr == '\n' || src.chr == '\r')
        {
            src.NextChr();
        }

        src.Start();
        start_off = src.GetCurrentOffset();

        /* possible identifier */
        if (isalpha(src.chr))
        {
            src.NextChr();
            Ident();   
            return true;
        }

        switch (src.chr) {
            case SENTINEL:     // EOF
                tokens.push_back(Token(TokenType::_EOF, "EOF", start_off, src.GetCurrentOffset()));
                return false;
            case '"':
                src.NextChr();
                bool ok;
                String(ok);
                if (!ok)
                {
                    // TODO(ashu3103): error
                    return false;
                }
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                src.NextChr();
                Number();
                break;
            case '+':
                src.NextChr();
                tokens.push_back(Token(TokenType::_PLUS, "+", start_off, start_off));
                break;
            case '-':
                src.NextChr();
                tokens.push_back(Token(TokenType::_DASH, "-", start_off, start_off));
                break;
            case '*':
                src.NextChr();
                tokens.push_back(Token(TokenType::_STAR, "*", start_off, start_off));
                break;
            case '/':
                src.NextChr();
                tokens.push_back(Token(TokenType::_SLASH, "/", start_off, start_off));
                break;
            case '(':
                src.NextChr();
                tokens.push_back(Token(TokenType::_LEFT_PAREN, "(", start_off, start_off));
                break;
            case '{':
                src.NextChr();
                tokens.push_back(Token(TokenType::_LEFT_BRACE, "{", start_off, start_off));
                break;
            case ')':
                src.NextChr();
                tokens.push_back(Token(TokenType::_RIGHT_PAREN, ")", start_off, start_off));
                break;
            case '}':
                src.NextChr();
                tokens.push_back(Token(TokenType::_RIGHT_BRACE, "}", start_off, start_off));
                break;
            case ',':
                src.NextChr();
                tokens.push_back(Token(TokenType::_COMMA, ",", start_off, start_off));
                break;
            case ';':
                src.NextChr();
                tokens.push_back(Token(TokenType::_SEMICOLON, ";", start_off, start_off));
                break;
            case '!':
                src.NextChr();
                end_off = src.GetCurrentOffset();
                if (src.chr == '=')
                {
                    src.NextChr();
                    tokens.push_back(Token(TokenType::_BANG_EQUAL, "!=", start_off, end_off));
                }
                else
                {
                    tokens.push_back(Token(TokenType::_BANG, "!", start_off, start_off));
                }
                break;
            case '>':
                src.NextChr();
                end_off = src.GetCurrentOffset();
                if (src.chr == '=')
                {
                    src.NextChr();
                    tokens.push_back(Token(TokenType::_GREATER_EQUAL, ">=", start_off, end_off));
                }
                else
                {
                    tokens.push_back(Token(TokenType::_GREATER, ">", start_off, start_off));
                }
                break;
            case '<':
                src.NextChr();
                end_off = src.GetCurrentOffset();
                if (src.chr == '=')
                {
                    src.NextChr();
                    tokens.push_back(Token(TokenType::_LESS_EQUAL, "<=", start_off, end_off));
                }
                else
                {
                    tokens.push_back(Token(TokenType::_LESS, "<", start_off, start_off));
                }
                break;
            case '=':
                src.NextChr();
                end_off = src.GetCurrentOffset();
                if (src.chr == '=')
                {
                    src.NextChr();
                    tokens.push_back(Token(TokenType::_EQUAL_EQUAL, "==", start_off, end_off));
                }
                else
                {
                    tokens.push_back(Token(TokenType::_EQUAL, "=", start_off, start_off));
                }
                break;
            default:
                break;
        }

        return true;
    }

    void Scanner::Ident()
    {
        std::string token = "";
        TokenType type = _IDENTIFIER;
        // read till a character/digit is encountered
        while (isalpha(src.chr) || isdigit(src.chr))
        {
            src.NextChr();
        }

        end_off = src.GetCurrentOffset() - 1;
        /* possibly a keyword */
        token = src.Segment();
        if (Token::IsKeyword(token))
        {
            type = keywordMap.at(token);
        }

        this->tokens.push_back(Token(type, token, start_off, end_off));
    }

    void Scanner::Number()
    {
        std::string token = "";
        TokenType type = _NUMBER;
        // read till a character is encountered
        while (isdigit(src.chr))
        {
            src.NextChr();
        }

        if (src.chr == '.')
        {
            src.NextChr();
            while (isdigit(src.chr)) {
                src.NextChr();
            }
        }

        token = src.Segment();
        /*
            It may happen that user provides 23. (that's it),
            compiler assumes this as a number and append a 0 at the back
        */
        if (token[token.size() - 1] == '.')
        {
            token += '0';
        }
        end_off = src.GetCurrentOffset() - 1;
        tokens.push_back(Token(type, token, start_off, end_off));
    }

    void Scanner::String(bool &ok)
    {
        ok = true;

        for (;;)
        {
            if (src.chr == SENTINEL) {     // EOF error
                ok = false;
                break;
            } else if (src.chr == '\n') {  // newline error
                src.NextChr();
                ok = false;
                break;
            } else if (src.chr == '"') {
                src.NextChr();
                end_off = src.GetCurrentOffset() - 1;
                tokens.push_back(Token(TokenType::_STRING, src.Segment(), start_off, end_off));
                ok = true;
                break;
            } else {
                src.NextChr();
            }
        }
    }

}