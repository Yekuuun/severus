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

//--------
//PARSING

/**
 * Parse tokens into a command base struct for later execution.
 * @param pTokens => PTR to PTOKEN struct (AST)
 */
PCOMMAND ParseTokens(IN PTOKEN pTokens){
    if(pTokens == NULL)
        return NULL;
    
    PCOMMAND head     = NULL;
    PCOMMAND current  = NULL;

    while(pTokens){
        if(pTokens->type == TOKEN_WORD){
            if(!current){
                current = (PCOMMAND)malloc(sizeof(COMMAND));
                RtlSecureZeroMemory(current, sizeof(COMMAND));
                head = current;
            }

            current->args = realloc(current->args, sizeof(CHAR*) * (current->argc + 2));
            current->args[current->argc] = strdup(pTokens->cmd);
            current->args[++(current->argc)] = NULL;
        }
        else if (pTokens->type == TOKEN_PIPE) {
            current->next = (PCOMMAND)malloc(sizeof(COMMAND));
            RtlSecureZeroMemory(current, sizeof(COMMAND));
            current = current->next;
        } 
        else if (pTokens->type == TOKEN_REDIRECT_OUT || pTokens->type == TOKEN_APPEND) {
            pTokens = pTokens->next;
            if (pTokens) {
                current->outputFile = strdup(pTokens->cmd);
                current->append = (pTokens->type == TOKEN_APPEND);
            }
        }
        else if(pTokens->type == TOKEN_REDIRECT_IN){
            pTokens = pTokens->next;
            if(pTokens)
                current->inputFile = strdup(pTokens->cmd);
        }

        pTokens = pTokens->next;
    }

    return head;
}

/**
 * FREE MEMORY.
 * @param pCommands => ptr to PCOMMAND struct
 */
VOID FreeCommands(IN PCOMMAND pCommands){
    if(pCommands == NULL)
        return;
    
    while(pCommands){
        PCOMMAND next = pCommands->next;

        for(DWORD i = 0; i < pCommands->argc; i++)
            free(pCommands->args[i]);
        
        free(pCommands->args);

        if(pCommands->inputFile)
            free(pCommands->inputFile);
        
        if(pCommands->outputFile)
            free(pCommands->outputFile);
        
        free(pCommands);

        pCommands = next;
    }
}