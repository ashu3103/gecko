#include <parser.h>
#include <core/gtype.h>
#include <error/errors.h>

template <typename... Args>
std::string Format(const Args&... args) {
    std::string out = "";
    (out = out + args);
    return out;
}

/* public methods */
namespace ast {
    Parser::Parser(std::vector<Token> t)
    {
        tokens.clear();
        tokens = t;

        current = 0;
    }

    Parser::~Parser() {
        tokens.clear();
    }

    std::vector<Stmt> Parser::Parse() {
        std::vector<Stmt> statements = {};
        while (!IsAtEnd()) {
            statements.push_back(NewStatement());
        }

        return statements;
    }
}

/* private helper methods */
namespace ast {
    bool Parser::Match(std::vector<TokenType> types) {
        for (int i = 0; i < types.size(); i++)
        {
            if (Check(types[i]))
            {
                Advance();
                return true;
            }
        }

        return false;
    }

    // ensure that we are currently scanning the token of a particular type
    Token Parser::Consume(TokenType type, std::string message) {
        if (Check(type)) return Advance();

        throw Error(errors::ErrorType::UNEXPECTED_TOKEN, Peek(), message);
    }

    // check if the current token is of particular type
    bool Parser::Check(TokenType type) {
        if (IsAtEnd()) return false;
        return Peek().type == type;
    }

    // consider next token for scanning
    Token Parser::Advance() {
        if (!IsAtEnd()) current++;
        return Previous();
    }

    // check if the parser has reached the end-of-file
    bool Parser::IsAtEnd() {
        return Peek().type == TokenType::_EOF;
    }

    // return the token currently under scanning
    Token Parser::Peek() {
        return tokens[current];
    }

    // return the last token that has been scanned
    Token Parser::Previous() {
        return tokens[current - 1];
    }

    ParserError Parser::Error(errors::ErrorType err_type, Token token, std::string msg) {
        errors::ReportError(err_type, token.pos, msg);
        return ParserError();
    }

    // Even in the event of an error, we want our parser to continue processing.
    // For this, the parser will attempt to reach the next potential statement by scanning tokens 
    // that indicate the beginning of a statement in the event that an error occurs while parsing 
    // for a statement.
    void Parser::Synchronize() {
        Advance();
        while (!IsAtEnd())
        {
            if (Previous().type == TokenType::_SEMICOLON) return;

            switch (Peek().type) {
                case TokenType::_CLASS:
                case TokenType::_FUN:
                case TokenType::_VAR:
                case TokenType::_FOR:
                case TokenType::_IF:
                case TokenType::_WHILE:
                case TokenType::_PRINT:
                case TokenType::_RETURN:
                    return;
            }
            Advance();
        }
    }
}

/* expressin parsing methods */
namespace ast {

    Stmt Parser::NewStatement() {
        try {
            if (Match({_VAR})) return VarDeclStmt();
            if (Match({_PRINT})) return PrintStmt();

            return ExpressionStmt();
        } catch (ParserError err) {
            Synchronize();
            return new Void();
        }
    }

    Stmt Parser::VarDeclStmt() {
        Token name = Consume(_IDENTIFIER, "Expect variable name.");

        Expr initializer = new Noop();
        if (Match({_EQUAL})) {
            initializer = NewExpression();
        }

        Consume(_SEMICOLON, "Expect ';' after variable declaration.");
        return new Var(name, initializer);
    }
    
    Stmt Parser::ExpressionStmt() {
        Expr value = NewExpression();
        Consume(_SEMICOLON, "Expect ';' after value.");
        return new Expression(value);
    }

    Stmt Parser::PrintStmt() {
        Expr value = NewExpression();
        Consume(_SEMICOLON, "Expect ';' after value.");
        return new Print(value);
    }

    Expr Parser::NewExpression()
    {
        return AssignmentExpr();
    }

    Expr Parser::AssignmentExpr() {
        Expr expr = EqualityExpr();

        if (Match({_EQUAL}))
        {
            Token equals = Previous();
            Expr value = AssignmentExpr();

            if (core::is_type<Variable*>(expr)) {
                Token name = (std::get<Variable*>(expr))->name;
                return new Assign(name, value);
            }

            Error(errors::ErrorType::UNEXPECTED_TOKEN , equals, "Invalid assignment target.");
        }

        return expr;
    }

    Expr Parser::EqualityExpr() {
        Expr expr = ComparsionExpr();

        if (Match({_BANG_EQUAL, _EQUAL_EQUAL})) {
            Token oper = Previous();
            Expr rhs = ComparsionExpr();
            expr = new Binary(expr, oper, rhs);
        }

        return expr;
    }

    Expr Parser::ComparsionExpr() {
        Expr expr = TermExpr();

        if (Match({_GREATER, _GREATER_EQUAL, _LESS, _LESS_EQUAL})) {
            Token oper = Previous();
            Expr rhs = TermExpr();
            expr = new Binary(expr, oper, rhs);
        }

        return expr;
    }

    Expr Parser::TermExpr() {
        Expr expr = FactorExpr();

        while (Match({_PLUS, _DASH})) {
            Token oper = Previous();
            Expr rhs = FactorExpr();
            expr = new Binary(expr, oper, rhs);
        }

        return expr;
    }

    Expr Parser::FactorExpr() {
        Expr expr = UnaryExpr();

        while (Match({_STAR, _SLASH})) {
            Token oper = Previous();
            Expr rhs = UnaryExpr();
            expr = new Binary(expr, oper, rhs);
        }

        return expr;
    }

    Expr Parser::UnaryExpr() {
        if (Match({_DASH, _BANG}))
        {
            Token oper = Previous();
            Expr rhs = Parser::UnaryExpr();
            return new Unary(oper, rhs);
        }

        return Primary();
    }

    Expr Parser::Primary()
    {
        if (Match({_TRUE, _FALSE, _NIL, _NUMBER, _STRING}))
        {
            return new Literal(Previous().tok);
        }

        if (Match({_IDENTIFIER}))
        {
            return new Variable(Previous());
        }

        if (Match({_LEFT_PAREN}))
        {
            Expr expr = NewExpression();
            Consume(_RIGHT_PAREN, "Expect ')' after expression.");
            return new Grouping(expr);
        }

        throw Error(errors::ErrorType::UNEXPECTED_TOKEN, Peek(), Format("Unexpected token ", Peek().tok, "."));

    }
}