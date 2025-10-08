#include "main.h"
#include "tokenizer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *head = NULL;
char *trailingHead = NULL;
char *startOfLine = NULL;
Token *last = NULL;
Token *tokenList = NULL;
size_t line = 0;

// creates a token based off of the state of global vars
Token *createToken(TokenType type){
    Token *tempToken = malloc(sizeof(Token));

    tempToken->length = head - trailingHead;
    tempToken->offset = trailingHead - startOfLine;
    tempToken->line = line;
    tempToken->type = type;
        
    tempToken->lexeme = malloc(sizeof(char) * (tempToken->length) + 1);
    memcpy(tempToken->lexeme, trailingHead, tempToken->length);
    tempToken->lexeme[tempToken->length] = '\0';

    tempToken->next = NULL;

    return tempToken;
}

bool detectWhitespace(){
    switch(*head){
        case '\n':
        case '\t':
        case '\r':
        case ' ':
            return true;
            break;
        default:
            return false;
            break;
    }
}

// skip \n \t ' ' etc
void skipWhitespace(){
    while(detectWhitespace()){
        if(*head == '\n'){
            startOfLine = head + 1;

            line++;
        }
        head++;
    }
}

void skipComments(){
    // detect and skip comment lines
    if(*head == '/' && *(head + 1) == '/'){   // '//' comments
        while(*head != '\n'){
            head++;
        }
    }
    if(*head == '/' && *(head + 1) == '*'){     // '/* ... */' comments
        while(*head == '*' && *(head + 1) == '/'){
            head++;
        }
    }
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlphanumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

Token *tokenify(const char *buffer, size_t length){
    head = trailingHead = startOfLine = (char *)buffer;

    while(*head != '\0'){  // while the head is not at the end of the buffer (should be EOF)
        skipWhitespace();
        skipComments();

        if(*head == '\0')
            break;

        trailingHead = head;
        TokenType type = TOKEN_IDENT;

        if(isAlpha(*head)){
            while(isAlphanumeric(*head))
                head++;
            
            type = TOKEN_IDENT;
        }
        else if(isDigit(*head)){
            while(isDigit(*head))
                head++;

            type = TOKEN_INT;
        }

        Token *newToken = createToken(type);
        if(tokenList == NULL){
            tokenList = newToken;
            last = tokenList;
        }
        else{
            last->next = newToken;
            last = newToken;
        }
    }

    trailingHead = head;
    Token *newToken = createToken(TOKEN_EOF);
    last->next = newToken;
    last = newToken;

    return tokenList;
}