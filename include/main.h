#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

typedef enum TokenType{
	TOKEN_IDENT = 0,
    
    // Literals
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_STRING,

    // Operators / punctuation
    TOKEN_DOT,      // .
    TOKEN_ARROW,    // ->
    TOKEN_EQUAL,    // =
    TOKEN_EQ,       // ==
    TOKEN_PLUS,     // +
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_LBRACE,   // {
    TOKEN_RBRACE,   // }
    TOKEN_LBRACK,   // [
    TOKEN_RBRACK,   // ]
    TOKEN_COMMA,    // ,
    TOKEN_SEMI,     // ;
    TOKEN_EOF       // '\0'
} TokenType;

typedef struct Token{
	char *lexeme;
	size_t length;

	size_t line;
    size_t offset;

    TokenType type;

    struct Token *next;
} Token;

#endif
