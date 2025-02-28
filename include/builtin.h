#pragma once
#include "global.h"
#include "history.h"

#define HASH_TABLE_BUILTINS_SIZE 10

//----
typedef struct BUILTINS {
    CHAR *cName;
    void (*func)(char **args);
} BUILTINS, *PBUILTINS;

//FUNCTIONS.
VOID InitBuiltins();
VOID ExitSeverus();
VOID ExecBuiltin(IN CHAR **args);
BOOL isBuiltin(IN CHAR *cStr);
VOID ClearConsole();