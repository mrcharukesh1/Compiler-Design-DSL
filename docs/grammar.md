# DSL Grammar (BNF)

## Formal Grammar

```
program   ::= 'begin' stmt_list 'end'

stmt_list ::= ε
            | stmt_list stmt

stmt      ::= IDENT '=' expr ';'
            | 'print' expr ';'

expr      ::= expr '+' expr
            | expr '-' expr
            | expr '*' expr
            | expr '/' expr
            | '(' expr ')'
            | INTEGER
            | IDENT

INTEGER   ::= [0-9]+
IDENT     ::= [a-zA-Z_][a-zA-Z0-9_]*
```

## Operator Precedence (low → high)

| Precedence | Operators | Associativity |
|------------|-----------|---------------|
| 1 (lowest) | `+` `-`   | Left          |
| 2          | `*` `/`   | Left          |

Parentheses override precedence as usual.

## Token Summary

| Token       | Pattern              |
|-------------|----------------------|
| `begin`     | keyword              |
| `end`       | keyword              |
| `print`     | keyword              |
| INTEGER     | `[0-9]+`             |
| IDENT       | `[a-zA-Z_][a-zA-Z0-9_]*` |
| `=`         | assignment           |
| `;`         | statement terminator |
| `+` `-` `*` `/` | arithmetic operators |
| `(` `)`     | grouping             |
| `#...`      | single-line comment  |
