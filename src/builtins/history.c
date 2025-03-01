/**
 * Author : Yekuuun
 * Github : https://github.com/yekuuun
 * 
 * Note : manage all history logic.
 */

#include "history.h"

static PCOMMANDHISTORY pHistory = NULL;

/**
 * Initialize command history.
 */
PCOMMANDHISTORY InitializeHistory(IN INT maxSize){
    pHistory = (PCOMMANDHISTORY)malloc(sizeof(COMMANDSHISTORY));
    if(pHistory == NULL)
        return NULL;
    
    pHistory->head = pHistory->tail = pHistory->current = NULL;
    pHistory->size = 0;
    pHistory->maxSize = maxSize;

    return pHistory;
}

/**
 * Add command to history.
 */
VOID AddToHistory(IN PCOMMANDHISTORY pHistory, IN LPCSTR pCmd){
    if(pHistory == NULL)
        return;
    
    PHISTORYNODE newNode = (PHISTORYNODE)malloc(sizeof(HISTORYNODE));
    if(newNode == NULL)
        return;
    
    newNode->command = strdup(pCmd);
    newNode->next = newNode->prev = NULL;

    if(pHistory->size == 0){
        pHistory->head = pHistory->tail = newNode;
        newNode->next = newNode->prev = newNode;
    }
    else {
        newNode->prev = pHistory->tail;
        newNode->next = pHistory->head;
        
        pHistory->tail->next = newNode;
        pHistory->head->prev = newNode;
        pHistory->tail = newNode;
    }

    if(pHistory->size < pHistory->maxSize){
        pHistory->size++;
    }
    else {
        PHISTORYNODE oldHead = pHistory->head;
        pHistory->head = oldHead->next;
        pHistory->head->prev = pHistory->tail;
        pHistory->tail->next = pHistory->head;

        free(oldHead->command);
        free(oldHead);
    }

    pHistory->current = NULL;
}

/**
 * Free struct.
 */
VOID FreeHistory(IN PCOMMANDHISTORY pHistory){
    if (pHistory == NULL) return;

    if (pHistory->size > 0) {
        PHISTORYNODE current = pHistory->head;
        PHISTORYNODE next;

        // Briser la boucle circulaire avant d’itérer
        pHistory->tail->next = NULL;

        while (current) {
            next = current->next;
            free(current->command);
            free(current);
            current = next;
        }
    }

    free(pHistory);
}

/**
 * Show inner history commands.
 */
VOID History(){
    if (pHistory == NULL || pHistory->size == 0)
        return;

    PHISTORYNODE current = pHistory->head;
    DWORD index = 1;

    printf("\n Command history : \n");
    printf(" ------- -------\n");
    do {
        printf("  %d %s\n", index, current->command);
        current = current->next;
        index++;
    } while (current != pHistory->head);
    printf("\n");
}
