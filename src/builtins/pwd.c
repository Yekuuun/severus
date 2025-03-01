#include "builtin.h"

/**
 * PWD command.
 */
VOID Pwd(){
    LPSTR lpPath = (LPSTR)malloc(MAX_PATH * sizeof(char));
    if(lpPath == NULL)
        return;

    DWORD dwPath = GetCurrentDirectoryA(MAX_PATH, lpPath);
    if(dwPath != 0)
        printf("%s\n", lpPath);
}