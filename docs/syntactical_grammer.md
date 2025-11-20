# Syntactical Grammer

A context-free grammer is a language whose alphabets are all the valid lexems and the string are the expressions made out of those lexems.

### Grammer of Expressions

For now, we are going to worry about only a handful of expressions:

- Literals. Numbers, strings, Booleans, and nil.
- Unary expressions. A prefix ! to perform a logical not, and - to negate a number.
- Binary expressions. The infix arithmetic (+, -, *, /) and logic operators (==, !=, <, <=, >, >=) we know and love.
- Parentheses. A pair of ( and ) wrapped around an expression.

```
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ("or" logic_and)* ;
logic_and      → equality ("and" equality)* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | call ;
call           → primary ( "(" arguments? ")" )* ;
arguments      → expression ( "," expression )* ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" | IDENTIFIER;
```

### Associativity

| Name | Oerator | Associates |
|:---|:---:|---:|
| Equality | ==, != | Left |
| Comparison | >, >=, <, <= | Left |
| Term | -, + | Left |
| Factor | /, * | Left |
| Unary | !, - | Right |


### Grammer of Statements

```
program        → declaration* EOF ;

declaration    → varDecl
               | funDecl
               | statement ;

statement      → exprStmt
               | printStmt
               | block
               | whileStmt
               | forStmt
               | ifStmt
               | returnStmt ;

returnStmt     → "return" expression? ";" ;

forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                 expression? ";"
                 expression? ")" statement ;

block          → "{" declaration* "}" ;

ifStmt         → "if" "(" expression ")" statment ("else" statement)? ;

whileStmt      → "while" "(" expression ")" statement ;

varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;

funDecl        → "fun" function ;
function       → IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
```