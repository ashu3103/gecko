#include <iostream>
#include <string>
#include <fstream>
#include <token.h>
#include <vector>
#include <scanner.h>
#include <parser.h>
#include <printer.h>
#include <expr.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        exit(1);
    }

    // lexing
    scanner::Scanner sc(argv[1]);
    while (sc.NextToken());
    std::vector<token::Token> tokens = sc.tokens;
    for (int i=0;i<tokens.size();i++)
    {
        std::cout << tokens[i].tok << std::endl;
    }

    ast::Expr lhs = new ast::Literal("1");
    ast::Expr rhs = new ast::Literal("2");
    ast::Expr root = new ast::Binary(lhs, token::Token(_PLUS, "+", 0, 0), rhs);

    ast::AstPrinter printer;
    std::cout << printer.print(root) << std::endl;;

    return 0;
}