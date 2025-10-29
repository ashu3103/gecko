#include <parser.h>
#include <error/errors.h>

using namespace ast;

/* Expression functions */
static Expr equality(Parser* p);
static Expr comparision(Parser* p);
static Expr term(Parser* p);
static Expr factor(Parser* p);
static Expr unary(Parser* p);
static Expr primary(Parser* p);

/* Statement functions */
static Stmt declaration(Parser* p);
static Stmt varDecl(Parser* p);
static Stmt statement(Parser* p);
static Stmt printStatement(Parser* p);
static Stmt exprStatement(Parser* p);

namespace ast {
    Parser::Parser(std::vector<token::Token> t) {
        this->tokens = t;
    }

    Parser::~Parser() {
        this->tokens.clear();
    }

    Token Parser::Previous()
    {
        return this->tokens[this->current - 1];
    }

    Token Parser::Current()
    {
        return this->tokens[this->current];
    }

    bool Parser::Match(std::vector<TokenType> matchList)
    {
        for (int i = 0; i < matchList.size(); i++)
        {
            if (tokens[current].type == matchList[i])
                return true;
        }

        return false;
    }

    void Parser::Advance(std::vector<TokenType> followList = {}) {
        /* compute the  follow set */
        followList.push_back(TokenType::_EOF);
        for (int i = 0; i < stopset.size(); i++)
        {
            followList.push_back(stopset[i]);
        }

        while (!Match(followList))
        {
            current++;
        }
    }

    bool Parser::Got(TokenType type) {
        if (current < tokens.size() && type == tokens[current].type)
        {
            current++;
            return true;
        }
        return false;
    }

    bool Parser::Want(TokenType type) {
        if (!Got(type))
        {
            if (!Token::IsOperator(type))
            {
                throw std::runtime_error("operator type is desired");
            }

            std::string msg = "expected " + operatorMap[type] + ", got " + Current().tok;
            errors::ReportError(errors::ErrorType::MISSING_TOKEN, Current().pos, msg);
            Advance({});
            return false;
        }

        return true;
    }

    Expr Parser::Expression() {
        return equality(this);
    }

    Stmt Parser::Statement() {
        return declaration(this);
    }
}

/* declaration = varDecl | statement */
static Stmt declaration(Parser* p) {
    if (p->Got(_VAR)) return varDecl(p);
    return statement(p);
}

static Stmt varDecl(Parser* p) {
    if (p->Got(_IDENTIFIER))
    {
        Expr expr = new Noop();
        if (p->Got(_EQUAL))
        {
            expr = p->Expression();
        }

        if (!p->Want(_SEMICOLON))
        {
            return new Void();
        }
        return new Var(p->Previous(), expr);
    }

    std::string msg = "exected an identifier got '" + p->Current().tok + "'";
    errors::ReportError(errors::ErrorType::UNEXPECTED_TOKEN, p->Current().pos, msg);
    p->Advance({});
    return new Void();
}

static Stmt statement(Parser* p) {
    if (p->Got(_PRINT))
    {
        return printStatement(p);
    }

    return exprStatement(p);
}

static Stmt printStatement(Parser* p)
{
    Expr e = p->Expression();
    if (p->Want(_SEMICOLON))
    {
        return new Void();
    }
    return new Print(e);
}

static Stmt exprStatement(Parser* p)
{
    Expr e = p->Expression();
    if (p->Want(_SEMICOLON))
    {
        return new Void();
    }
    return new Expression(e);
}

/* equality = comparision( (== | !=) comparision); */
static Expr equality(Parser* p) {
    Expr lhs = comparision(p);
    if (p->Got(_BANG_EQUAL) || p->Got(_EQUAL_EQUAL))
    {
        Token tok = p->Previous();
        Expr rhs = comparision(p);
        return new Binary(lhs, tok, rhs);
    }
    return lhs;
}

/* comparision = term (( < | <= | > | >= ) term); */
static Expr comparision(Parser* p) {
    Expr lhs = term(p);
    if (p->Got(_LESS) || p->Got(_LESS_EQUAL) || p->Got(_GREATER) || p->Got(_GREATER_EQUAL))
    {
        Token tok = p->Previous();
        Expr rhs = term(p);
        return new Binary(lhs, tok, rhs);
    }
    return lhs;
}

/* term = factor ((+ | -) factor)*; */
static Expr term(Parser* p)
{
    Expr ex = factor(p);

    while (p->Got(_PLUS) || p->Got(_DASH))
    {
        Token tok = p->Previous();
        Expr rhs = factor(p);
        ex = new Binary(ex, tok, rhs);
    }
    return ex;
}

/* factor = unary ((* | /) unary)*; */
static Expr factor(Parser* p)
{
    Expr ex = unary(p);
    while (p->Got(_STAR) || p->Got(_SLASH))
    {
        Token tok = p->Previous();
        Expr rhs = unary(p);
        ex = new Binary(ex, tok, rhs);
    }
    return ex;
}

/* uanry = (! | -) unary | primary; */
static Expr unary(Parser* p)
{
    if (p->Got(_DASH) || p->Got(_BANG))
    {
        Token tok = p->Previous();
        Expr rhs = unary(p);
        return new Unary(tok, rhs);
    }
    return primary(p);
}

/* primary = NUMBER | STRING | "true" | "false" | "nil" | IDENTIFIER
               | "(" expression ")" ; */
static Expr primary(Parser* p)
{   
    if (p->Got(_TRUE) || p->Got(_FALSE) || p->Got(_NUMBER) || p->Got(_STRING) || p->Got(_NIL)) {
        return new Literal(p->Previous().tok);
    }

    if (p->Got(_IDENTIFIER)) {
        return new Variable(p->Previous().tok);
    }

    if (p->Got(_LEFT_PAREN)) {
        Expr expr = p->Expression();
        /* Noop */
        if (expr.index() == 4)
        {
            goto error;
        }
        if (!p->Got(_RIGHT_PAREN))
        {
            errors::ReportError(errors::ErrorType::MISSING_TOKEN, p->Current().pos, "Missing token ')'");
            goto error;
        }
        return new Grouping(expr);
    }

    errors::ReportError(errors::ErrorType::UNEXPECTED_TOKEN, p->Current().pos, "Unexpected Token");
error:
    p->Advance({});
    return new Noop();
}