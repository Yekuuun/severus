#include "builtin.h"

static CHAR previousDir[MAX_PATH] = "";


/**
 * Check path validity.
 */
static BOOL IsValidPath(IN CHAR *cPath){
    SIZE_T len = strlen(cPath);
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
VOID Cd(IN CHAR **args){
    CHAR cNewPath[MAX_PATH] = {0};
    CHAR currentDir[MAX_PATH] = {0};

    if (!GetCurrentDirectoryA(MAX_PATH, currentDir))
        return;

    if (args[1] == NULL) {
        CHAR *home = getenv("USERPROFILE");
        if (!home) 
            return;
        
        strncpy(cNewPath, home, MAX_PATH);
        cNewPath[MAX_PATH - 1] = '\0';
    } 
    else if (strcmp(args[1], "..") == 0) {
        CHAR parentDir[MAX_PATH] = {0};
        strncpy(parentDir, currentDir, MAX_PATH);
        parentDir[MAX_PATH - 1] = '\0';

        CHAR *lastSlash = strrchr(parentDir, '\\');
        if (lastSlash) {
            *lastSlash = '\0';
        }
        strncpy(cNewPath, parentDir, MAX_PATH);
    } 
    else {
        if (!GetFullPathNameA(args[1], MAX_PATH, cNewPath, NULL)) {
            printf("[!] Error resolving path '%s'.\n", args[1]);
            return;
        }
    }

    if (IsValidPath(cNewPath) || !SetCurrentDirectoryA(cNewPath)) {
        printf("[!] Incorrect path...\n");
        return;
    }

    strncpy(previousDir, currentDir, MAX_PATH);
    previousDir[MAX_PATH - 1] = '\0';
}
