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

    if (errors::has_errors)
    {
        exit(1);
    }

    ast::Parser parser(tokens);
    std::vector<ast::Stmt> stmts = parser.Parse();
    // ast::Stmt stmt1 = parser.NewStatement();
    ast::AstPrinter printer;
    for (int i = 0; i < stmts.size(); i++)
    {
        std::cout << i << ": " << printer.print(stmts[i]) << std::endl;
    }

    return 0;
}