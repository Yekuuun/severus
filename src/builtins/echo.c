#include "builtin.h"

/**
 * Echo command.
 */
VOID Echo(IN CHAR **args){
    DWORD dwSize = GetArrayCharSize(args);
    if(dwSize == 0 || dwSize == 1)
        return;

    INT newLine = 1;
    INT startIndex = 1;

    if(strcmp(args[1], "-n") == 0){
        newLine = 0;
        startIndex = 2;
    }

    //display arguments.
    for(DWORD i = startIndex; i < dwSize; i++){
        printf("%s", args[i]);
        if(i < dwSize - 1)
            printf(" ");
    }

    if(newLine == 1)
        printf("\n");
    
    return;
}