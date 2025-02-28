/**
 * Author : Yekuuun
 * Github : https://github.com/yekuuun
 * 
 * Notes  : provide all pre built shell commands.
 */

#include "builtin.h"

static PBUILTINS builtins[HASH_TABLE_BUILTINS_SIZE];

//---------BUILTIN FUNCTIONS------------

/**
 * Display current DIR path.
 */
VOID CustomPWD(){
    LPSTR lpPath[MAX_PATH] = {0};

    DWORD dwPath = GetCurrentDirectoryA(MAX_PATH, lpPath);
    if(dwPath != 0)
        printf("%s\n", lpPath);
}

/**
 * Check path validity.
 */
static BOOL IsValidPath(IN CHAR *cPath){
    INT len = strlen(cPath);
    if(len == 0 || cPath[0] == '\0')
        return FALSE; 
    
    if(len >= MAX_PATH)
        return FALSE;
    
    LPCSTR invalidChars = "\\:*?\"<>|";
    if(strpbrk(cPath, invalidChars) != NULL)
        return FALSE;
    
    DWORD dwAttributes = GetFileAttributes((LPCSTR)cPath);
    if(dwAttributes != INVALID_FILE_ATTRIBUTES && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY))
        return TRUE;
    
    return FALSE;
}

/**
 * Change current directory.
 */
VOID Cd(IN CHAR *cPath){
    if(!IsValidPath(cPath) || !SetCurrentDirectoryA((LPCSTR)cPath)){
        printf("[!] Path incorrect.\n");
        return;
    }
}


/**
 * Clearing console = BUILTIN CLS.
 */
VOID ClearConsole() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == NULL)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hStdOut, &dwMode))
        return;

    const DWORD dwOriginalMode = dwMode;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hStdOut, dwMode))
        return;

    DWORD dwWritten = 0;
    PCWSTR pwSeq = L"\x1b[2J\x1b[H";  // Efface et remet le curseur en haut

    if (!WriteConsoleW(hStdOut, pwSeq, (DWORD)wcslen(pwSeq), &dwWritten, NULL)) {
        SetConsoleMode(hStdOut, dwOriginalMode);
        return;
    }

    SetConsoleMode(hStdOut, dwOriginalMode);
}

//------

/**
 * Ending program.
 */
VOID ExitSeverus(){
    printf("$ exiting severus. bye...\n");
    if(!TerminateProcess(GetCurrentProcess(), 0)){
        exit(0);
    }
}

//----------------------------------------------------

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
    RtlSecureZeroMemory((PVOID)builtins, sizeof(builtins));

    AddBuiltin("clean", ClearConsole, builtins);
    AddBuiltin("exit", ExitSeverus, builtins);
    AddBuiltin("history", ShowHistory, builtins);
    AddBuiltin("pwd", CustomPWD, builtins);
    AddBuiltin("cd", Cd, builtins);
}

