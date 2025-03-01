#pragma once
#include "global.h"
#include "history.h"
#include "libft.h"

#define HASH_TABLE_BUILTINS_SIZE 10

//----
typedef struct BUILTINS {
    CHAR *cName;
    void (*func)(char **args);
} BUILTINS, *PBUILTINS;

//FUNCTIONS.
VOID InitBuiltins();
VOID ExecBuiltin(IN CHAR **args);
BOOL isBuiltin(IN CHAR *cStr);

//builtins
VOID Clean();
VOID Exit(IN CHAR **args);
VOID Pwd();
VOID Cd(IN CHAR **args);
VOID Echo(IN CHAR **args);