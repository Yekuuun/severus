/**
 * Author : Yekuuun
 * Github : https://github.com/Yekuuun
 * 
 * Notes : Ls base implementation => link to man : https://man7.org/linux/man-pages/man1/ls.1.html
 */

#include "cmd.h"

/**
 * Initialize LS logic.
 */
static VOID InitializeArgs(){
    return;
}

/**
 * Base ls command => no args.
 */
static VOID LsBase(){
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFileA("*", &findData);
    
    if(hFind == INVALID_HANDLE_VALUE){
        printf("[!] An error occurred while listing files.\n");
        return;
    }

    do {
        SIZE_T sLen = strlen(findData.cFileName);
        CHAR *cName = (CHAR*)malloc(sLen + 2); // +2 pour '/' et '\0'
        
        if(!cName)
            break;

        strcpy(cName, findData.cFileName);
        
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            strcat(cName, "/");

        printf("%s\n", cName);
        free(cName);
        
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
}

/**
 * LS ENTRY POINT.
 */
int main(int argc, char **argv){
    if(argc > 2){
        printf("[!] Error calling ls command => consult man for help.\n");
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}