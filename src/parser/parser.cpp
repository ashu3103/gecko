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

    Stmt Parser::DesugarForLoop(Stmt initializer, Expr condition, Expr increment, Stmt b) {
        Stmt body = b;
        /* desugaring for loop to while */
        if (core::is_type<Noop*>(increment))
        {
            std::vector<Stmt> v = {};
            v.push_back(body);
            v.push_back(new Expression(increment));

            body = new Block(v);
        }

        // if condition is not provided, infinite loop
        if (core::is_type<Noop*>(condition)) condition = new Literal("true");
        body = new While(condition, body);

        if (!core::is_type<Void*>(initializer))
        {
            std::vector<Stmt> v = {};
            v.push_back(initializer);
            v.push_back(body);

            body = new Block(v); 
        }

        return body;
    }
}

/* expressin parsing methods */
namespace ast {

    Stmt Parser::NewStatement() {
        try {
            if (Match({_VAR})) return VarDeclStmt();
            if (Match({_PRINT})) return PrintStmt();
            if (Match({_LEFT_BRACE})) return BlockStmt();
            if (Match({_IF})) return IfStmt();
            if (Match({_WHILE})) return WhileStmt();
            if (Match({_FOR})) return ForStmt();

            return ExpressionStmt();
        } catch (ParserError err) {
            Synchronize();
            return new Void();
        }
    }

    Stmt Parser::ForStmt() {
        Consume(_LEFT_PAREN, "Expect '(' after 'for'");
        /* calculate initializer */
        Stmt initializer;
        if (Match({_SEMICOLON})) // skip initializer
        {
            initializer = new Void();
        }
        else if (Match({_VAR}))
        {
            initializer = VarDeclStmt();
        }
        else
        {
            initializer = ExpressionStmt();
        }

        /* calculate initializer */
        Expr condition = new Noop();
        if (!Check(_SEMICOLON))
        {
            condition = NewExpression();
        }
        Consume(_SEMICOLON, "Expect ';' after loop condition.");

        /* calculate increment */
        Expr increment = new Noop();
        if (!Check(_RIGHT_PAREN)) {
            increment = NewExpression();
        }
        Consume(_RIGHT_PAREN, "Expect ')' after for clauses.");
        Stmt body = NewStatement();

        return DesugarForLoop(initializer, condition, increment, body);
    }

    Stmt Parser::WhileStmt() {
        Consume(_LEFT_PAREN, "Expect '(' after 'while'");
        Expr condition = NewExpression();
        Consume(_RIGHT_BRACE, "Expect ')' after 'while' condition");

        Stmt body = NewStatement();
        return new While(condition, body);
    }

    Stmt Parser::BlockStmt() {
        std::vector<Stmt> statements = {};

        while (!Check(_RIGHT_BRACE) && !IsAtEnd()) {
            statements.push_back(NewStatement());
        }

        Consume(_RIGHT_BRACE, "Expect '}' after block");
        new Block(statements);
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

    Stmt Parser::IfStmt() {
        Consume(_LEFT_PAREN, "Expect '(' after 'if'.");
        Expr condition = NewExpression();
        Consume(_RIGHT_PAREN, "Expect ')' after 'if' condition.");

        Stmt thenBranch = NewStatement();
        Stmt elseBranch = new Void();

        if (Match({_ELSE}))
        {
            elseBranch = NewStatement();
        }
        return new If(condition, thenBranch, elseBranch);
    }

    Expr Parser::NewExpression()
    {
        return AssignmentExpr();
    }

    Expr Parser::AssignmentExpr() {
        Expr expr = OrExpr();

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

    Expr Parser::OrExpr() {
        Expr expr = AndExpr();

        while (Match({_OR})) {
            Token oper = Previous();
            Expr right = AndExpr();
            expr = new Logical(expr, oper, right);
        }

        return expr;
    }

    Expr Parser::AndExpr() {
        Expr expr = EqualityExpr();

        while (Match({_AND})) {
            Token oper = Previous();
            Expr right = EqualityExpr();
            expr = new Logical(expr, oper, right);
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

        return CallExpr();
    }

    Expr Parser::CallExpr() {
        Expr expr = Primary();
        while (true)
        {
            if (Match({_LEFT_PAREN})) {
                expr = finishCall(expr);
            } else {
                break;
            }
        }

        return expr;
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
    
    Expr Parser::FinishCall(Expr callee) {
        std::vector<Expr> arguments = {};
        if (!Check(_RIGHT_PAREN))
        {
            do {
                if (arguments.size() >= 255)
                {
                    Error(errors::ErrorType::DIVISION_BY_ZERO, Peek(), "Can't have more than 255 arguments.");
                }
                arguments.push_back(NewExpression());
            } while(Match({_COMMA}));
        }

        Token paren = Consume(_RIGHT_PAREN, "Expect a ')' after arguments");
        return new Call(callee, paren, arguments);
    }
}
