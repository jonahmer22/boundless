#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

typedef struct Token Token;

Token *tokenify(const char *buffer, size_t length);

#endif
