#include <expr.h>
#include <stmt.h>
#include <token.h>

/* system */
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace token;

const std::vector<TokenType> stopset = {
    TokenType::_BREAK,
    TokenType::_CONTINUE,
    TokenType::_FOR,
    TokenType::_IF,
    TokenType::_RETURN,
    TokenType::_VAR,
    TokenType::_WHILE,
    TokenType::_EOF,
    TokenType::_PRINT
};

namespace ast {
    class ParserError: public std::exception {
        public:
            const char* what() const noexcept override {
                return "Parser Error!";
            }
    };

    class Parser {
        private:
            /* states of a parser */
            std::vector<Token> tokens = {};
            int current = 0;
            bool fnest = false;

            /* private helper methods */
            bool Match(std::vector<TokenType> types);
            Token Consume(TokenType type, std::string message);
            bool Check(TokenType type);
            Token Advance();
            bool IsAtEnd();
            Token Peek();
            Token Previous();
            void Synchronize();
            ParserError Error(errors::ErrorType err_type, Token token, std::string msg);

            /* expression parsing methods */
            Expr NewExpression();
            Expr AssignmentExpr();
            Expr EqualityExpr();
            Expr ComparsionExpr();
            Expr TermExpr();
            Expr FactorExpr();
            Expr UnaryExpr();
            Expr Primary();

            /* statement parsing method */
            Stmt NewStatement();
            Stmt VarDeclStmt();
            Stmt ExpressionStmt();
            Stmt PrintStmt();

        public:

            Parser(std::vector<Token> t);
            ~Parser();
            
            std::vector<Stmt> Parse();
    };
}