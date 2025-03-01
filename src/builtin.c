/**
 * Author : Yekuuun
 * Github : https://github.com/yekuuun
 * 
 * Notes  : Provide base builtin commands handling.
 */

#include "builtin.h"

static PBUILTINS builtins[HASH_TABLE_BUILTINS_SIZE] = {0};

/**
 * Hash function name.
 */
static UINT HashFunction(IN CHAR *cStr){
    UINT hash = 0;
    while(*cStr){
        hash += *cStr++;
    }

    return hash % HASH_TABLE_BUILTINS_SIZE;
}

/**
 * Adding builins commands.
 */
static VOID AddBuiltin(IN CHAR *cName, IN VOID(*func)(char **), IN PBUILTINS *pTable){
    UINT index    = HashFunction(cName);
    PBUILTINS cmd = (PBUILTINS)malloc(sizeof(BUILTINS));
    if(cmd == NULL)
        return;
    
    cmd->cName    = cName;
    cmd->func     = func;
    pTable[index] = cmd;
}

/**
 * Execute builtins commands.
 */
VOID ExecBuiltin(IN CHAR **args){
    UINT index = HashFunction(args[0]);
    if(builtins[index] && strcmp(builtins[index]->cName, args[0]) == 0){
        builtins[index]->func(args);
    }
}

/**
 * Check if exist in builtins.
 */
BOOL isBuiltin(IN CHAR *cStr){
    UINT index = HashFunction(cStr);
    PBUILTINS cmd = builtins[index];

    return builtins[index] != NULL && (strcmp(builtins[index]->cName, cStr) == 0);
}

/**
 * Initialize builtins commands.
 */
VOID InitBuiltins(){
    AddBuiltin("clean", Clean, builtins);
    AddBuiltin("exit", Exit, builtins);
    AddBuiltin("history", History, builtins);
    AddBuiltin("pwd", Pwd, builtins);
    AddBuiltin("cd", Cd, builtins);
    AddBuiltin("echo", Echo, builtins);
}


