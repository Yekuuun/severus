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
    if(pToken == NULL);
        return NULL;
    
    pToken->cmd = cmd;
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
        while(temp->next){
            temp = temp->next;
        }

        temp->next = pNew;
    }
}