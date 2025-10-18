#include <parser.h>
#include <gtype.h>
#include <error/errors.h>

using namespace ast;

static Expr equality(Parser* p);
static Expr comparision(Parser* p);
static Expr term(Parser* p);
static Expr factor(Parser* p);
static Expr unary(Parser* p);
static Expr primary(Parser* p);

namespace ast {
    Parser::Parser(std::vector<token::Token>&t) {
        this->tokens = t;
    }

    Parser::~Parser() {
        this->tokens.clear();
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
        if (type != tokens[current].type)
        {
            current++;
            return true;
        }
        return false;
    }

    void Parser::Want(TokenType type) {
        if (!Got(type))
        {
            // report error
            Advance({});
        }
    }

    Expr Parser::Expression() {
        return equality(this);
    }

}

/* equality = comparision( (== | !=) comparision); */
static Expr equality(Parser* p) {
    Expr lhs = comparision(p);
    if (p->Match({_BANG_EQUAL, _EQUAL_EQUAL}))
    {
        Token tok = p->tokens[p->current];
        p->current++;
        Expr rhs = comparision(p);
        return new Binary(lhs, tok, rhs);
    }
    return lhs;
}

/* comparision = term (( < | <= | > | >= ) term); */
static Expr comparision(Parser* p) {
    Expr lhs = term(p);
    if (p->Match({_LESS, _LESS_EQUAL, _GREATER, _GREATER_EQUAL}))
    {
        Token tok = p->tokens[p->current];
        p->current++;
        Expr rhs = term(p);
        return new Binary(lhs, tok, rhs);
    }
    return lhs;
}

/* term = factor ((+ | -) factor)*; */
static Expr term(Parser* p)
{
    Expr ex = factor(p);

    while (p->Match({_PLUS, _DASH}))
    {
        Token tok = p->tokens[p->current];
        p->current++;
        std::cout << tok.tok << std::endl;
        Expr rhs = factor(p);
        ex = new Binary(ex, tok, rhs);
        p->current++;
    }
    return ex;
}

/* factor = unary ((* | /) unary)*; */
static Expr factor(Parser* p)
{
    Expr ex = unary(p);
    while (p->Match({_STAR, _SLASH}))
    {
        Token tok = p->tokens[p->current];
        p->current++;
        Expr rhs = unary(p);
        ex = new Binary(ex, tok, rhs);
    }
    return ex;
}

/* uanry = (! | -) unary | primary; */
static Expr unary(Parser* p)
{
    if (p->Match({_DASH, _BANG}))
    {
        Token tok = p->tokens[p->current];
        p->current++;
        Expr rhs = unary(p);
        return new Unary(tok, rhs);
    }
    return primary(p);
}

/* primary = NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ; */
static Expr primary(Parser* p)
{
    Token tok = p->tokens[p->current];
    
    if (p->Match({_TRUE, _FALSE, _NIL, _NUMBER, _STRING})) {
        p->current++;
        return new Literal(tok.tok);
    }

    p->current++;
    if (tok.type == _LEFT_PAREN) {
        Expr expr = p->Expression();
        if (!p->Got(_RIGHT_PAREN))
        {
            goto error;
        }
        return new Grouping(expr);
    }
error:
    errors::ReportError(errors::ErrorType::UNEXPECTED_TOKEN, tok.pos, "Unexpected Token");
    p->Advance();
    return new Noop();
}