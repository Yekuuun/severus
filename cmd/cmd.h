/**
 * Author : Yekuuun
 * Github : https://github.com/yekuuun
 * 
 * Notes  : contains all utils for CMD commands implementation (struct, function, etc...)
 */
#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_ARGS_SIZE 30

//STRUCTS.
typedef struct ARGSHANDLER {
    CHAR *cOption;
    VOID (*func)(CHAR **args);
} ARGSHANDLER, *PARGSHANDLER;

//FUNCTIONS.

/**
 * Create hash for input CHAR*
 * @param cInput => char to hash.
 */
__forceinline UINT HashFunction(IN CHAR* cInput){
    if(cInput == NULL)
        return 0;

    UINT hash = 0;
    while(*cInput){
        hash += *cInput++;
    }

    return hash % HASH_TABLE_ARGS_SIZE;
}

/**
 * Adding argument to table.
 */
__forceinline VOID AddArg(IN CHAR *cName, IN VOID(*func)(CHAR **), IN PARGSHANDLER *pTable){
    if(!cName || !func || !pTable)
        return; 

    UINT hash = HashFunction(cName);
    PARGSHANDLER pArgs = (PARGSHANDLER)malloc(sizeof(ARGSHANDLER));
    if(!pArgs)
        return;
    
    pArgs->cOption = cName;
    pArgs->func    = func;
    pTable[hash]   = pArgs;
}

/**
 * Check if exist in argument table. 
 * @param cArg => char to find
 * @param pArgs => ptr to ARGSHANDER table.
 */
__forceinline BOOL isArgs(IN CHAR *cArg, IN PARGSHANDLER *pArgs){
    UINT index = HashFunction(cArg);
    PARGSHANDLER pArg = pArgs[index];
    return pArgs[index] != NULL && (strcmp(pArgs[index]->cOption, cArg) == 0);
}

/**
 * Exec option function.
 */
__forceinline VOID ExecOption(IN CHAR **args, IN PARGSHANDLER *pTable){
    if(args[0] == NULL)
        return;
    
    UINT index = HashFunction(args[0]);
    if(pTable[index] && strcmp(pTable[index]->cOption, args[0]) == 0){
        pTable[index]->func(args);
    }
    else {
        printf("$ command not found.\n");
    }
}

//-------

//UTILS.

/**
 * Check if its an option.
 */
__forceinline BOOL IsOption(IN CHAR *arg){
    return (arg[0] == '-');
}

/**
 * Check if value is a valid path.
 */
__forceinline BOOL IsPath(IN CHAR *arg){
    if(IsOption(arg))
        return FALSE;
    
    if (strncmp(arg, "./", 2) == 0 || strncmp(arg, "../", 3) == 0)
        return TRUE;
    
    if ((strlen(arg) > 2) && arg[1] == ':' && (arg[2] == '\\' || arg[2] == '/'))
        return TRUE;
}
