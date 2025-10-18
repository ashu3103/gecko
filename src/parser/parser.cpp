// #include <parser.h>
// #include <gtype.h>
// #include <error/errors.h>

// using namespace ast;

// template <typename R>
// static Expr<R>* equality(Parser* p);
// template <typename R>
// static Expr<R>* comparision(Parser* p);
// template <typename R>
// static Expr<R>* term(Parser* p);
// template <typename R>
// static Expr<R>* factor(Parser* p);
// template <typename R>
// static Expr<R>* unary(Parser* p);
// template <typename R>
// static Expr<R>* primary(Parser* p);

// namespace ast {
//     Parser::Parser(std::vector<token::Token>&t) {
//         this->tokens = t;
//     }

//     Parser::~Parser() {
//         this->tokens.clear();
//     }

//     bool Parser::Match(std::vector<TokenType> matchList)
//     {
//         for (int i = 0; i < matchList.size(); i++)
//         {
//             if (tokens[current].type == matchList[i])
//                 return true;
//         }

//         return false;
//     }

//     void Parser::Advance(std::vector<TokenType> followList = {}) {
//         /* compute the  follow set */
//         followList.push_back(TokenType::_EOF);
//         for (int i = 0; i < stopset.size(); i++)
//         {
//             followList.push_back(stopset[i]);
//         }

//         while (!Match(followList))
//         {
//             current++;
//         }
//     }

//     bool Parser::Got(TokenType type) {
//         if (type != tokens[current].type)
//         {
//             current++;
//             return true;
//         }
//         return false;
//     }

//     void Parser::Want(TokenType type) {
//         if (!Got(type))
//         {
//             // report error
//             Advance({});
//         }
//     }

//     template <typename R>
//     Expr<R>* Parser::Expression() {
//         return equality<R>(this);
//     }

//     /* explicit declaration */
//     template ast::Expr<std::string>* Parser::Expression<std::string>(void);
// }

// /* equality = comparision( (== | !=) comparision); */
// template <typename R>
// static Expr<R>* equality(Parser* p) {
//     Expr<R>* lhs = comparision<R>(p);
//     if (p->Match({_BANG_EQUAL, _EQUAL_EQUAL}))
//     {
//         Token tok = p->tokens[p->current];
//         p->current++;
//         Expr<R>* rhs = comparision<R>(p);
//         return new Binary<R>(lhs, tok, rhs);
//     }
//     return lhs;
// }

// /* comparision = term (( < | <= | > | >= ) term); */
// template <typename R>
// static Expr<R>* comparision(Parser* p) {
//     Expr<R>* lhs = term<R>(p);
//     if (p->Match({_LESS, _LESS_EQUAL, _GREATER, _GREATER_EQUAL}))
//     {
//         Token tok = p->tokens[p->current];
//         p->current++;
//         Expr<R>* rhs = term<R>(p);
//         return new Binary<R>(lhs, tok, rhs);
//     }
//     return lhs;
// }

// /* term = factor ((+ | -) factor)*; */
// template <typename R>
// static Expr<R>* term(Parser* p)
// {
//     Expr<R>* ex = factor<R>(p);
//     printf("term expression address: %p\n", ex);
//     while (p->Match({_PLUS, _DASH}))
//     {
//         Token tok = p->tokens[p->current];
//         p->current++;
//         std::cout << tok.tok << std::endl;
//         Expr<R>* rhs = factor<R>(p);
//         printf("term expression address: %p\n", rhs);
//         ex = new Binary<R>(ex, tok, rhs);
//         p->current++;
//         printf("binary expression address: %p\n", ex);
//     }
//     return ex;
// }

// /* factor = unary ((* | /) unary)*; */
// template <typename R>
// static Expr<R>* factor(Parser* p)
// {
//     Expr<R>* ex = unary<R>(p);
//     while (p->Match({_STAR, _SLASH}))
//     {
//         Token tok = p->tokens[p->current];
//         p->current++;
//         Expr<R>* rhs = unary<R>(p);
//         ex = new Binary<R>(ex, tok, rhs);
//     }
//     return ex;
// }

// /* uanry = (! | -) unary | primary; */
// template <typename R>
// static Expr<R>* unary(Parser* p)
// {
//     if (p->Match({_DASH, _BANG}))
//     {
//         Token tok = p->tokens[p->current];
//         p->current++;
//         Expr<R>* rhs = unary<R>(p);
//         return new Unary<R>(rhs, tok);
//     }
//     return primary<R>(p);
// }

// /* primary = NUMBER | STRING | "true" | "false" | "nil"
//                | "(" expression ")" ; */
// template <typename R>
// static Expr<R>* primary(Parser* p)
// {
//     Token tok = p->tokens[p->current];
//     core::gtype value;
//     std::cout << "inside primary " << tok.type << " " << tok.tok << std::endl;
//     if (p->Match({_TRUE})) {
//         value = true;
//         p->current++;
//         return new Literal<R>(value);
//     }
//     if (p->Match({_FALSE})){
//         value = false;
//         p->current++;
//         return new Literal<R>(value);
//     }
//     if (p->Match({_NIL})){
//         value = "nil";
//         p->current++;
//         return new Literal<R>(value);
//     }

//     if (p->Match({_NUMBER, _STRING})){
//         value = tok.tok;
//         p->current++;
//         return new Literal<R>(value);
//     }

//     if (tok.type == _LEFT_PAREN) {
//         Expr<R>* expr = p->Expression<R>();
//         if (!p->Got(_RIGHT_PAREN))
//         {
//             goto error;
//         }
//         return new Grouping<R>(expr);
//     }
// error:
//     errors::ReportError(errors::ErrorType::UNEXPECTED_TOKEN, tok.pos, "Unexpected Token");
//     p->Advance();
//     return NULL;
// }

// template static ast::Expr<std::string>* primary<std::string>(ast::Parser* p);