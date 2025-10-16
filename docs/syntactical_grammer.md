# Syntactical Grammer

A context-free grammer is a language whose alphabets are all the valid lexems and the string are the expressions made out of those lexems.

### Grammer of Expressions

For now, we are going to worry about only a handful of expressions:

- Literals. Numbers, strings, Booleans, and nil.
- Unary expressions. A prefix ! to perform a logical not, and - to negate a number.
- Binary expressions. The infix arithmetic (+, -, *, /) and logic operators (==, !=, <, <=, >, >=) we know and love.
- Parentheses. A pair of ( and ) wrapped around an expression.

```
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
```

### Associativity

| Name | Oerator | Associates |
|:---|:---:|---:|
| Equality | ==, != | Left |
| Comparison | >, >=, <, <= | Left |
| Term | -, + | Left |
| Factor | /, * | Left |
| Unary | !, - | Right |


