#pragma once
#include "global.h"

#define MAX_HISTORY 20 //MAX NODES IN HISTORY.

//ROUNDED DOUBLE LINKED LIST.
typedef struct HISTORYNODE {
    char *command; 
    struct HISTORYNODE* prev;
    struct HISTORYNODE* next;
} HISTORYNODE, *PHISTORYNODE;

//-----

typedef struct COMMANDSHISTORY {
    PHISTORYNODE current;
    PHISTORYNODE head;
    PHISTORYNODE tail;
    INT size;
    INT maxSize;
} COMMANDSHISTORY, *PCOMMANDHISTORY;

//functions.
PCOMMANDHISTORY InitializeHistory(IN INT maxSize);
VOID AddToHistory(IN PCOMMANDHISTORY pHistory, IN LPCSTR pCmd);
VOID FreeHistory(IN PCOMMANDHISTORY pHistory);
VOID ShowHistory();