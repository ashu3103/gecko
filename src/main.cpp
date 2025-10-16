#include <iostream>
#include <string>
#include <fstream>
#include <token.h>
#include <printer.h>
#include <expr.h>

int main(int argc, char* argv[])
{
    // create a token
    token::Token tok(TokenType::_BANG, "!", 0, 0);
    // create a literal
    core::gtype l = "valid";
    ast::Literal<std::string> litExpr(l);
    

    // typecast the unary expr
    ast::Expr<std::string>* ex = new ast::Unary<std::string>(&litExpr, tok);
    ast::AstPrinter p;
    
    std::cout << ex->accept(p) << std::endl;

    return 0;
}