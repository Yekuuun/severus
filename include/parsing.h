#pragma once
#include "global.h"
#include "libft.h"

/**
 * Identifies tokens.
 */
typedef enum ETOKENTYPE {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF,
} ETOKENTYPE;

/**
 * Single token structure.
 */
typedef struct TOKEN {
    DWORD dwIndex;
    CHAR *cmd;
    ETOKENTYPE type;
    struct TOKEN *next;
} TOKEN, *PTOKEN;

//functions
VOID Lexer(IN CHAR *input, IN OUT PTOKEN *tokens);
VOID FreeTokens(IN PTOKEN tokens);