/**
 * Author : Yekuuun
 * Github : https://github.com/Yekuuun
 * 
 * Notes  : Base parsing lib. => cmd parsing.
 */

#include "parsing.h"

/**
 * Creating a new token.
 * @param cmd => CHAR command
 * @param eTYpe => Type of token
 * @param dwIndex => number of the new token for the token list.
 */
static PTOKEN CreateToken(IN CHAR *cmd, IN ETOKENTYPE eType, IN DWORD dwIndex){
    PTOKEN pToken = (PTOKEN)malloc(sizeof(TOKEN));
    if(pToken == NULL)
        return NULL;
    
    pToken->cmd = Ft_Strndup(cmd, strlen(cmd));  
    if (pToken->cmd == NULL) {  
        free(pToken);
        return NULL;
    }

    pToken->dwIndex = dwIndex;
    pToken->type = eType;
    pToken->next = NULL;

    return pToken;
}

/**
 * Adding a new token to the list.
 * @param head => head of the token list.
 * @param cmd => new cmd to add to the list
 * @param eType => type of new token
 * @param dwCount => counter for the new token in the list.
 */
static VOID AddToken(IN PTOKEN *head, IN CHAR *cmd, IN ETOKENTYPE eType, IN DWORD dwCount){
    PTOKEN pNew = CreateToken(cmd, eType, dwCount);
    if(pNew == NULL)
        return;
    
    if(*head == NULL){
        *head = pNew;
    }
    else {
        PTOKEN temp = *head;
        while(temp->next)
            temp = temp->next;

        temp->next = pNew;
    }
}

/**
 * Main lexer function.
 * @param input => base command to lex
 * @param tokens => base PTOKENS ptr.
 */
VOID Lexer(IN CHAR *input, IN OUT PTOKEN *tokens) {
    DWORD i = 0;
    DWORD index = 1;
    
    while (input[i]) {
        CHAR current = input[i];
        if (isspace(current)) {
            i++;
            continue;
        }
        else if (current == '|') {
            AddToken(tokens, "|", TOKEN_PIPE, index);
            i++;
            index++;
        }
        else if (current == '>') {
            if (input[i + 1] == '>') {
                AddToken(tokens, ">>", TOKEN_APPEND, index);
                i += 2;
            } else {
                AddToken(tokens, ">", TOKEN_REDIRECT_OUT, index);
                i++;
            }
            index++;
        }
        else if (current == '<') {
            if (input[i + 1] == '<') {
                AddToken(tokens, "<<", TOKEN_HEREDOC, index);
                i += 2;
            } else {
                AddToken(tokens, "<", TOKEN_REDIRECT_IN, index);
                i++;
            }
            index++;
        }
        else {
            int start = i;
            while (input[i] && !isspace(input[i]) && input[i] != '|' && input[i] != '>' && input[i] != '<')
                i++;

            CHAR *word = Ft_Strndup(&input[start], i - start);
            if (word) {
                AddToken(tokens, word, TOKEN_WORD, index);
            }

            index++;
        }
    }
}

/**
 * Free memory.
 */
VOID FreeTokens(IN PTOKEN tokens) {
    while (tokens) {
        PTOKEN next = tokens->next;
        if (tokens->cmd)
            free(tokens->cmd);

        free(tokens);
        tokens = next;
    }
}

