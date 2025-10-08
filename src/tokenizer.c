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

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlphanumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

static void skipWhitespaceAroundHead(){
    while(*head != '\0' && detectWhitespace()){
        if(*head == '\n'){
            startOfLine = head + 1;
            line++;
        }
        head++;
    }
}

static void skipCommentsAroundHead(){
    bool consumed = false;

    do{
        consumed = false;
        skipWhitespaceAroundHead();

        if(*head == '/' && *(head + 1) == '/'){
            consumed = true;
            head += 2;
            while(*head != '\0' && *head != '\n'){
                head++;
            }
            continue;
        }

        if(*head == '/' && *(head + 1) == '*'){
            consumed = true;
            head += 2;
            while(*head != '\0'){
                if(*head == '\n'){
                    line++;
                    startOfLine = head + 1;
                }

                if(*head == '*' && *(head + 1) == '/'){
                    head += 2;
                    break;
                }

                head++;
            }
            continue;
        }
    }while(consumed);
}

Token *tokenify(const char *buffer, size_t length){
    (void)length;

    head = trailingHead = startOfLine = (char *)buffer;
    tokenList = NULL;
    last = NULL;
    line = 0;

    while(*head != '\0'){  // while the head is not at the end of the buffer (should be EOF)
        skipCommentsAroundHead();
        skipWhitespaceAroundHead();

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
        else{
            head++;
            continue;
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
    if(last != NULL){
        last->next = newToken;
        last = newToken;
    }
    else{
        tokenList = newToken;
        last = newToken;
    }

    return tokenList;
}
